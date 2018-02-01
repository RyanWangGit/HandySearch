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
#include <QDir>
#include "html.h"


/**
* Class:    InvertedList
*
* Brief:    This class controls several local inverted lists, send requery and load signals,
* receive signals from local inverted lists, in order to do multi-thread trick.
*
* Date:    Nov. 2015
*/
class InvertedList : public QObject
{
    Q_OBJECT
private:
    QList<QThread*> threadList;
    QList<QThread*> activeThreadList;
    QList<Html*> resultList;
    QStringList keyWordList;
    QStringList titleList;
    QDir htmlFolder;
    bool hasLoaded;
    /* Compare in descending order */
    static inline bool compareHtml(Html* h1, Html* h2){ return *h2 < *h1; };
public:
    InvertedList();
    ~InvertedList();
    void load();
    void setHtmlFolder(const QDir &htmlFolder);
    const QStringList &getTitleList() const;
    /* Signals/Slots for inner objects */
private slots:
    void localQueryResult(QThread *thread, const QList<Html*> &resultList);
    void localLoadFinished(QThread *thread, const QStringList& titleList);
signals:
    void localQuery(const QStringList &keyWordList);
    void localLoadStart();
    
    /* Signals/Slots for outer objects */
public slots:
    void query(const QStringList &keyWordList);
signals:
    void queryResult(const QList<Html*>&, const QStringList&);
    void htmlLoadStarted();
    void htmlLoadFinished();
};
