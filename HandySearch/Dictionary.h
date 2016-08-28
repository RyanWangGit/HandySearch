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
#include "BloomFilter.h"


/**
* Class:    Dictionary
*
* Brief:    This class is implemented as a interface to control the dictionary,
* which is the wrapper of BloomFilter.
*
* Date:    Oct. 2015
*/
class Dictionary : public QObject
{
    Q_OBJECT
private:
    BloomFilter bf;
    QDir dictFolder;
    unsigned int maxLength;
    bool hasLoaded;
public:
    Dictionary();
    void load();
    void setDictFolder(const QDir &dictFolder);
    bool hasItem(const QString &key) const;
    bool addItem(const QString &key);
    unsigned int getMaxLength() const;
signals:
    void dictLoadStarted();
    void dictLoaded(int num);
    void dictLoadFinished();
};