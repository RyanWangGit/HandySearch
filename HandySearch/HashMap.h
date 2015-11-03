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
 * Class:	Entry
 *
 * Brief:	An entry of HashMap,Key should always be QString.
 *
 * Date:	Oct. 2015
 */
template <typename V >
class Entry
{
public:
	QString key;
	V value;
	//Empty constructor for list heads
	Entry(){	void;	}
	//Constructor
	Entry(QString key, V value)
	{
		this->key = key;
		this->value = value;
	}
	//Copy constructor
	Entry(const Entry<V> &c)
	{
		this->key = c.key;
		this->value = c.value;
	}
	//Operator =
	Entry<V> &operator=(const Entry<V> &other)
	{
		if (this == &other)
			return *this;
		this->key = other.key;
		this->value = other.value;
		return *this;
	}
};

/**
 * Class:	HashMap
 *
 * Brief:	HashMap is used as inverted list,provides mainly put and get method,
 * provides thread-safe read-write.
 *
 * Date:	Oct. 2015
 */
template <typename V >
class HashMap
{
#define INDEX_SIZE 1024000 //2^20
private:
	List<Entry<V>> *index[INDEX_SIZE];
	float loadFactor;
	int loadNum;
	QMutex mutex;

public:
	HashMap();
	~HashMap();
	bool put(const QString &key, V &value);
	V* get(const QString &key);
	bool resize(int newSize);

	/* Static methods */
	static unsigned int hashCode(char *key, int len);
};


template < typename V >
HashMap<V>::HashMap()
{
	loadFactor = 0;
	loadNum = 0;
	for (int i = 0; i < INDEX_SIZE; i++)
		index[i] = nullptr;
}


template < typename V >
HashMap<V>::~HashMap()
{
	for (int i = 0; i < INDEX_SIZE; i++)
		delete index[i];
}


template < typename V >
bool HashMap<V>::put(const QString &key, V &value)
{
	QByteArray ba = key.toLocal8Bit();
	char* str = ba.data();

	//Lock up to prevent from other thread to write
	mutex.lock();
	unsigned int i = HashMap::hashCode(str, ba.size()) % INDEX_SIZE;

	if (index[i] == nullptr)
	{
		index[i] = new List<Entry<V>>();
		loadNum++;
		loadFactor = (float)loadNum / INDEX_SIZE;
	}

	bool hasFound = false;
	List<Entry<V>>* bucket = index[i];
	for (int k = 0; k < bucket->size(); k++)
	{
		Entry<V> *temp = &bucket->get(k);
		if (temp->key == key)
		{
			temp->value = value;
			hasFound = true;
		}
	}
	if (!hasFound)
	{
		Entry<V> temp(key, value);
		bucket->append(temp);
	}

	//Unlock
	mutex.unlock();

	return true;
}


template < typename V >
V* HashMap<V>::get(const QString &key)
{
	QByteArray ba = key.toLocal8Bit();
	char* str = ba.data();

	unsigned int i = HashMap::hashCode(str, ba.size()) % INDEX_SIZE;

	if (index[i] == nullptr)
		return nullptr;

	mutex.lock();

	List<Entry<V>>* bucket = index[i];
	for (int k = 0; k < bucket->size(); k++)
	{
		Entry<V> *temp = &bucket->get(k);
		if (temp->key == key)
		{
			mutex.unlock();
			return &temp->value;
		}
	}

	mutex.unlock();
	return nullptr;
}

template < typename V >
bool HashMap<V>::resize(int newSize)
{
	return true;
}

/* BKDRHash function */
template < typename V >
unsigned int HashMap<V>::hashCode(char * str, int len)
{
	unsigned int seed = 131; /* 31 131 1313 13131 131313 etc.. */
	unsigned int hash = 0;
	unsigned int i = 0;

	for (i = 0; i < len; str++, i++)
	{
		hash = (hash * seed) + (*str);
	}

	return hash;
}