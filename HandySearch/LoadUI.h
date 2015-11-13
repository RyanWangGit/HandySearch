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
#include "stdafx.h"
#include "ui_loadui.h"

/**
 * Class:	LoadUI
 *
 * Brief:	The loading dialog class.
 *
 * Date:	Oct. 2015
 */
class LoadUI : public QDialog
{
	Q_OBJECT
private:
	QDir htmlFolder;
	QDir dictFolder;
	/* Loading clock */
	QTime clock;
	QTimer timer;
	/* For dragging the window */
	QPoint origin;
	bool isPressed;
	/* For showing current progress */
	unsigned long currentProgress;
	unsigned long maximumProgress;
	static LoadUI *instance;
	bool checkDirectory();
protected:
	/* Override event handler */
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
public:
	LoadUI();
	~LoadUI();
	static LoadUI *getInstance();
public slots:
	bool loadData();
	/* UI slots */
	void loadingDots();
	/* Load slots */
	void loadStarted();
	void loadFinished();
	/* Html load slots */
	void htmlLoadStarted();
	void htmlLoaded(int num);
	void htmlLoadFinished();
	/* Dictionary load slots */
	void dictLoadStarted();
	void dictLoaded(int num);
	void dictLoadFinished();
signals:
	void start();
	void canceled();
	void finished();
private:
	Ui::LoadUI ui;
};

