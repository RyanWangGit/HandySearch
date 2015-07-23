#pragma once
template < typename K, typename V >
class HashMap
{
#define INDEX_SIZE 1048576 //2^20
private:
	List<V> *index;
	float loadFactor;
public:
	HashMap();
	~HashMap();
	bool put(const K &key,const V &value);
	V& get(const K &key);
	bool resize(int newSize);

	/* Static methods */
	static unsigned int hashCode(const void * key, int len, const unsigned int seed = 0xEE6B27EB);
};

template < typename K, typename V >
HashMap<K, V>::HashMap()
{
	this->index = new List<V>[INDEX_SIZE];
	//TODO
}

template < typename K, typename V >
bool HashMap<K, V>::put(const K &key, const V &value)
{
	unsigned int index = HashMap::hashCode((void *)&key,)
		//TODO
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