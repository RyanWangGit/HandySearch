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

/**
 * Class:    BloomFilter
 *
 * Brief:    This BloomFilter is used as dictionary,
 * provides O(1) put and get effiency,with error rate
 * down to around 1% .
 *
 * Date:    Oct. 2015
 */
class BloomFilter
{
#define MAXSIZE 67108864 // 2^26
private:
    QBitArray bitArray;
    /* Hash functions */
    //14 Hash functions and 20 times of number of items length for bitArray
    //makes error rate 0.0001
    unsigned int APHash(const char* str, unsigned int len) const;
    unsigned int BKDRHash(const char* str, unsigned int len) const;
    unsigned int BPHash(const char* str, unsigned int len) const;
    unsigned int DEKHash(const char* str, unsigned int len) const;
    unsigned int DJBHash(const char* str, unsigned int len) const;
    unsigned int ELFHash(const char* str, unsigned int len) const;
    unsigned int FNVHash(const char* str, unsigned int len) const;
    unsigned int HFHash(const char* str, unsigned int len) const;
    unsigned int HFLPHash(const char* str, unsigned int len) const;
    unsigned int JSHash(const char* str, unsigned int len) const;
    unsigned int PJWHash(const char* str, unsigned int len) const;
    unsigned int RSHash(const char* str, unsigned int len) const;
    unsigned int SDBMHash(const char* str, unsigned int len) const;
    unsigned int StrHash(const char* str, unsigned int len) const;
    /* End of hash functions */
public:
    BloomFilter();
    bool hasItem(const void *key, int len) const;
    bool addItem(const void *key, int len);
};
