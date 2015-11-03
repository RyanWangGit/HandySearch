/*****************************************
* DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER.
*
* Copyright 2015 Ryan Wang, wangyuxin@hust.edu.cn
*
* This file is part of HandySearch.
* HandySearch is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* HandySearch is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with HandySearch.  If not, see <http://www.gnu.org/licenses/>.
*
* - Author: Ryan Wang
* - Contact: wangyuxin@hust.edu.cn
* - License: GNU Lesser General Public License (LGPL)
* - Blog and source code availability: http://ryanwanggit.github.io/HandySearch/
*****************************************/
#pragma once
#include "Html.h"
//#define SKIPLOAD


/**
 * Class:	HtmlLoadSubTask
 *
 * Brief:	Html load sub-task class.
 *
 * Date:	Nov. 2015
 */
class HtmlLoadSubTask : public QObject, public QRunnable
{
	Q_OBJECT
private:
	QStringList pathList;
public:
	HtmlLoadSubTask(const QStringList &pathList);

signals:
	void htmlLoaded();

public:
	void run();
};


/**
 * Class:	HtmlLoadTask
 *
 * Brief:	A html load task,which manages several html load sub-tasks
 * and merges all the local hashmap.
 *
 * Date:	Nov. 2015
 */
class HtmlLoadTask : public QObject
{
	Q_OBJECT
private:
	QObject* parent;
	QDir htmlFolder;
public:
	HtmlLoadTask(const QDir& htmlFolder, QObject * parent = 0);
	void load();
signals:
	void htmlLoadStarted();
	void htmlLoadFinished();
};


/**
 * Class:	DictLoadTask
 *
 * Brief:	A dictionary load task.
 *
 * Date:	Nov. 2015
 */
class DictLoadTask : public QObject
{
	Q_OBJECT
private:
	QDir dictFolder;
public:
	DictLoadTask(const QDir& dictFolder);
	void load();
signals:
	void dictLoadStarted();
	void dictLoaded(int num);
	void dictLoadFinished();
};