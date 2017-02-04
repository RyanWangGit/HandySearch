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
#include "List.h"
#include "Dictionary.h"
#include "Html.h"


/**
 * Class:    Chunk
 *
 * Brief:    A chunk consists of at most three word,it represents 
 * one possible segmentation.
 *
 * Date:    Oct. 2015
 */
class Chunk
{
private:
    QStringList words;
    int wordCount;
    int totalLen;
    double avgLen;
    double variance;
public:
    Chunk();
    Chunk(const Chunk & other);
    Chunk(QString fWord, QString sWord, QString tWord);
    int getCount();
    int getLength();
    double getAvgLen();
    double getVariance();
    QStringList &getWords();
    Chunk &operator=(const Chunk &other);
};


/**
 * Class:    WordSegmenter
 *
 * Brief:    Using MMSeg algorithm,thi class is implemented to 
 * do word segmentation.
 *
 * Date:    Oct. 2015
 */
class WordSegmenter
{
private:
    QString content;
    QStringList result;
    const Dictionary *dict;
    unsigned int pos;
    void mmFilter(List<Chunk> &chunks);
    void lawlFilter(List<Chunk> &chunks);
    void svwlFilter(List<Chunk> &chunks);
    void sdmfFilter(List<Chunk> &chunks);
    bool isChineseChar(const QChar &ch);
    QChar getNextChar() const;
    QStringList getMaxMatchingWord();
    QStringList getChineseWords();
    QString getASCIIWords();
    void createChunks(List<Chunk> &chunks);
public:
    WordSegmenter(const QString &content, const Dictionary *dict);
    const QStringList &getResult();
};

