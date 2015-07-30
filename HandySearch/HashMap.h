#pragma once
template < typename K, typename V >
class Entry
{
public:
	K key;
	V value;
	//Empty constructor for list heads
	Entry(){	void;	}
	//Constructor
	Entry(K key, V value)
	{
		this->key = key;
		this->value = value;
	}
	//Copy constructor
	Entry(const Entry<K, V> &c)
	{
		this->key = c.key;
		this->value = c.value;
	}
	//Operator =
	Entry<K, V> &operator=(const Entry<K, V> &other)
	{
		if (this == &other)
			return *this;
		this->key = other.key;
		this->value = other.value;
		return *this;
	}
};


template < typename K, typename V >
class HashMap
{
#define INDEX_SIZE 1048576 //2^20
private:
	List<Entry<K,V>> **index;
	float loadFactor;
public:
	HashMap();
	~HashMap();
	bool put(const K &key, unsigned int len, V &value);
	bool put(const QString &key, List<Index> &value);
	V& get(const K &key, unsigned int len);
	V& get(const QString &key);
	bool resize(int newSize);

	/* Static methods */
	static unsigned int hashCode(const void * key, int len, const unsigned int seed = 0xEE6B27EB);
};

template < typename K, typename V >
HashMap<K, V>::HashMap()
{
	//Allocate a pointer array
	this->index = new List<Entry<K,V>> *[INDEX_SIZE];
	//Set all pointer to null
	for (int i = 0; i < INDEX_SIZE; i++)
		this->index[i] = nullptr;
}

template < typename K, typename V >
HashMap<K, V>::~HashMap()
{
	for (int i = 0; i < INDEX_SIZE; i++)
		delete this->index[i];
	delete[] this->index;
}

template < typename K, typename V >
bool HashMap<K, V>::put(const K &key, unsigned int len, V &value)
{
	try
	{
		unsigned int i = HashMap::hashCode((void *)&key, len) % INDEX_SIZE;
		if (this->index[i] == nullptr)
			this->index[i] = new List<Entry<K, V>>();
		for (int k = 0; k < this->index[i]->size(); k++)
		{
			Entry<K, V> *temp = &this->index[i]->get(k);
			if (temp->key == key)
				temp->value = value;
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}

template < typename K, typename V >
bool HashMap<K, V>::put(const QString &key, List<Index> &value)
{
	QByteArray ba = key.toLocal8Bit();
	return this->put(ba.data(), ba.size(), value);
}


template < typename K, typename V >
V& HashMap<K, V>::get(const K &key,unsigned int len)
{
	try
	{
		unsigned int i = HashMap::hashCode((void *)&key, len) % INDEX_SIZE;
		for (int j = 0; j < this->index[i]->size(); j++)
		{
			if (this->index[i] == nullptr)
				return 0;
			for (int k = 0; k < this->index[i]->size(); k++)
			{
				Entry<K, V> *temp = &this->index[i]->get(k);
				if (temp->key == key)
					return temp->value;
			}
		}
	}
	catch (...)
	{
		return 0;
	}
	return 0;
}

template < typename K, typename V >
V& HashMap<K, V>::get(const QString &key)
{
	QByteArray ba = key.toLocal8Bit();
	this->get(ba.data(), ba.size());
}

template < typename K, typename V >
bool HashMap<K, V>::resize(int newSize)
{
	return true;
}

/* We use Murmur 2.0 hash function here */
/* and use 0xEE6B27EB as the seed */
template < typename K, typename V >
unsigned int HashMap<K, V>::hashCode(const void * key, int len, const unsigned int seed = 0xEE6B27EB)
{
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

	return h;
}