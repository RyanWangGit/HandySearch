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
#pragma  once
#include <QStringList>
#include "List.h"
#include "HashMap.h"
#include "Index.h"
#include "Html.h"

/**
* Class:    LocalInvertedList
*
* Brief:    Local invertedList consists of several html tasks and a local hashmap
* which contains the index of htmls.
*
* Date:    Nov. 2015
*/
class LocalInvertedList : public QObject
{
    Q_OBJECT
private:
    HashMap<List<Index>*> *localHashMap;
    QList<List<Index>*> hashMapContent;
    QStringList pathList;
    /* Segment the content and save it to the inverted list */
    void putInLocalList(Html *html);
public:
    LocalInvertedList(const QStringList &pathList);
    ~LocalInvertedList();
public slots:
    void localLoadStart();
    void localQuery(const QStringList &keyWordList);
signals:
    void htmlLoaded(int num);
    void localLoadFinished(QThread *, const QStringList &);
    void localQueryResult(QThread *, const QList<Html*> &);
};
