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
 * Brief:	HashMap is used as inverted list,provides mainly put and get method.
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
public:
	HashMap();
	~HashMap();
	bool put(const QString &key, V &value);
	V* get(const QString &key);
	bool resize(int newSize);
	void statistic();

	/* Static methods */
	static unsigned int hashCode(char *key, int len);// , const unsigned int seed = 0xEE6B27EB);
};

template <typename V >
void HashMap<V>::statistic()
{

}

template < typename V >
HashMap<V>::HashMap()
{
	//Allocate a pointer array
//	this->index = new List<Entry<K,V>>* [INDEX_SIZE];
	//Set all pointer to null
	this->loadFactor = 0;
	this->loadNum = 0;
	for (int i = 0; i < INDEX_SIZE; i++)
		this->index[i] = nullptr;
}

template < typename V >
HashMap<V>::~HashMap()
{
	for (int i = 0; i < INDEX_SIZE; i++)
		delete this->index[i];
	//delete this->index;
}

template < typename V >
bool HashMap<V>::put(const QString &key, V &value)
{
	QByteArray ba = key.toLocal8Bit();
	char* str = ba.data();
	try
	{
		unsigned int i = HashMap::hashCode(str, ba.size()) % INDEX_SIZE;
		if (this->index[i] == nullptr)
		{
			this->index[i] = new List<Entry<V>>();
			this->loadNum++;
			this->loadFactor = (float)this->loadNum / INDEX_SIZE;
		}

		bool hasFound = false;
		List<Entry<V>>* bucket = this->index[i];
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

	}
	catch (...)
	{
		return false;
	}
	return true;
}


template < typename V >
V* HashMap<V>::get(const QString &key)
{
	QByteArray ba = key.toLocal8Bit();
	char* str = ba.data();
	try
	{
		unsigned int i = HashMap::hashCode(str, ba.size()) % INDEX_SIZE;

		if (this->index[i] == nullptr)
			return nullptr;

		List<Entry<V>>* bucket = this->index[i];
		for (int k = 0; k < bucket->size(); k++)
		{
			Entry<V> *temp = &bucket->get(k);
			if (temp->key == key)
				return &temp->value;
		}
	}
	catch (...)
	{
		return nullptr;
	}
	return nullptr;
}

template < typename V >
bool HashMap<V>::resize(int newSize)
{
	return true;
}

/* We use Murmur 2.0 hash function here */
/* and use 0xEE6B27EB as the seed */
template < typename V >
unsigned int HashMap<V>::hashCode(char * str, int len)//, const unsigned int seed = 0xEE6B27EB)
{
	/*
	// 'm' and 'r' are mixing constants generated offline.
	// They're not really 'magic', they just happen to work well.

	const unsigned int m = 0x5bd1e995;
	const int r = 24;

	// Initialize the hash to a 'random' value

	unsigned int h = seed ^ len;

	// Mix 4 bytes at a time into the hash

	const unsigned char * data = (const unsigned char *)key;

	while (len >= 4)
	{
		unsigned int k = *(unsigned int *)data;

		k *= m;
		k ^= k >> r;
		k *= m;

		h *= m;
		h ^= k;

		data += 4;
		len -= 4;
	}

	// Handle the last few bytes of the input array

	switch (len)
	{
	case 3: h ^= data[2] << 16;
	case 2: h ^= data[1] << 8;
	case 1: h ^= data[0];
		h *= m;
	};

	// Do a few final mixes of the hash to ensure the last few
	// bytes are well-incorporated.

	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return h;*/
	unsigned int seed = 131; /* 31 131 1313 13131 131313 etc.. */
	unsigned int hash = 0;
	unsigned int i = 0;

	for (i = 0; i < len; str++, i++)
	{
		hash = (hash * seed) + (*str);
	}

	return hash;
}