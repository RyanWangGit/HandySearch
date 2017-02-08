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

#include "ui_HandySearch.h"
#include "BloomFilter.h"
#include "Index.h"
#include "LoadUI.h"
#include "InvertedList.h"
#include "Dictionary.h"

/**
 * Class:    HandySearch
 *
 * Brief:    Main QT class,manages all threads and global variables,posseses UI thread.
 *
 * Date:    Oct. 2015
 */
class HandySearch : public QMainWindow
{
    Q_OBJECT
public slots:
    void loadCanceled();
    void loadFinished();
    /* UI slots */
    void segment();
    void search();
    void searchResult(const QList<Html*> &resultList, const QStringList &keyWordList);
    void anchorClicked(const QUrl &url);
    void about();
public:
    HandySearch(QWidget *parent = 0);
    Dictionary *getDictionary();
    InvertedList *getInvertedList();
    static HandySearch *getInstance();
    bool load();
protected:
    void resizeEvent(QResizeEvent *event);
private:
    QTime clock;
    bool isResultShown;
    void setDefaultUILayout();
    void setResultUILayout();
    void showResult(const QList<Html*> &resultList, const QStringList &wordList);
    QCompleter* completer;
    static HandySearch* instance;
    Dictionary dictionary;
    InvertedList invertedList;
    QThread dictThread;
    QThread listThread;
    LoadUI loadUI;
    Ui::HandySearchClass ui;
    /* Constants */
    const int MINWIDTH;
    const int MINHEIGHT;
};

