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
//#define DEBUG


/**
 * Class:	LoadHtml
 *
 * Brief:	Html load sub-thread class.
 *
 * Date:	Oct. 2015
 */
class LoadHtml : public QObject
{
	Q_OBJECT
private:
	int id;
	QStringList pathList;
public:
	static unsigned int threadNum;
	LoadHtml(const QStringList &pathList);

signals:
	void processHtml(unsigned int, Html*, QString);
	void finished();

public slots:
	void load()
	{
#ifdef DEBUG
		qDebug() << "[Html Loading Thread #" << this->id << "]" << "Thread Received " << pathList.size() << "Files";
#endif 
		for (int i = 0; i < pathList.size(); i++)
		{
#ifndef SKIPLOAD 
			Html *pHtml = new Html(pathList[i]);
			emit processHtml(this->id, pHtml, pathList.at(i));
#endif
		}		
		emit finished();
	}
};


/**
 * Class:	Load
 *
 * Brief:	Initial Load thread,manages the dictionary loading
 * and html loading threadlets.
 *
 * Date:	Oct. 2015
 */
class Load : public QObject
{
	Q_OBJECT
private:
	QDir htmlFolder;
	QDir dictFolder;
public:
	Load(const QDir &htmlFolder, const QDir &dictFolder);

signals:
	void loadStarted();
	void loadFinished();
	//Html Thread Signals
	void htmlLoaded(unsigned int, QString);
	void htmlLoadStarted();
	void htmlLoadFinished();
	//Dictionary Thread Signals
	void dictLoaded(int num);
	void dictLoadStarted();
	void dictLoadFinished();

private slots:
	//Load tasks
	void loadDictionary();
	void loadHtml();

public slots:
	//Initial load thread
	void run()
	{
		connect(this, &Load::loadStarted, this, &Load::loadDictionary);
		connect(this, &Load::dictLoadFinished, this, &Load::loadHtml);
		connect(this, &Load::htmlLoadFinished, this, &Load::loadFinished);

		emit loadStarted();
	}
	void processHtml(unsigned int threadID,Html* html,QString path);
	void htmlThreadFinished();
};


