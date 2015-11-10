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
 * Class:	BloomFilter
 *
 * Brief:	This BloomFilter is used as dictionary,
 * provides O(1) put and get effiency,with error rate
 * down to around 1% .
 *
 * Date:	Oct. 2015
 */
class BloomFilter
{
#define MAXSIZE 67108864 // 2^26
private:
	QBitArray bitArray;
	/* Hash functions */
	//14 Hash functions and 20 times of number of items length for bitArray
	//makes error rate 0.0001
	unsigned int APHash(char* str, unsigned int len);
	unsigned int BKDRHash(char* str, unsigned int len);
	unsigned int BPHash(char* str, unsigned int len);
	unsigned int DEKHash(char* str, unsigned int len);
	unsigned int DJBHash(char* str, unsigned int len);
	unsigned int ELFHash(char* str, unsigned int len);
	unsigned int FNVHash(char* str, unsigned int len);
	unsigned int HFHash(char* str, unsigned int len);
	unsigned int HFLPHash(char* str, unsigned int len);
	unsigned int JSHash(char* str, unsigned int len);
	unsigned int PJWHash(char* str, unsigned int len);
	unsigned int RSHash(char* str, unsigned int len);
	unsigned int SDBMHash(char* str, unsigned int len);
	unsigned int StrHash(char* str, unsigned int len);
	/* End of hash functions */
public:
	BloomFilter();
	bool hasItem(void *key, int len);
	bool addItem(void *key, int len);
};
