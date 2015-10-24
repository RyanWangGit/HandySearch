#include "stdafx.h"
#include "BloomFilter.h"


/*--------------------------
* BloomFilter::BloomFilter
* 	The default constructor of BloomFilter.
----------------------------*/
BloomFilter::BloomFilter()
{
	this->bitArray.resize(MAXSIZE);
	this->maxLength = 0;
}



/*--------------------------
* BloomFilter::hasItem
* 	Find whether the item is in the BloomFilter or not.
* Returns:	bool - Result.
* Parameter:
*	void * key - The key to search.
*	int len - Length of string.
----------------------------*/
bool BloomFilter::hasItem(void *key, int len)
{
	return
		(
		this->bitArray.testBit(APHash((char *)key, len) % MAXSIZE)
		&& this->bitArray.testBit(BKDRHash((char *)key, len) % MAXSIZE)
		&& this->bitArray.testBit(BPHash((char *)key, len) % MAXSIZE)
		&& this->bitArray.testBit(DEKHash((char *)key, len) % MAXSIZE)
		&& this->bitArray.testBit(DJBHash((char *)key, len) % MAXSIZE)
		&& this->bitArray.testBit(ELFHash((char *)key, len) % MAXSIZE)
		&& this->bitArray.testBit(FNVHash((char *)key, len) % MAXSIZE)
		&& this->bitArray.testBit(HFHash((char *)key, len) % MAXSIZE)
		&& this->bitArray.testBit(HFLPHash((char *)key, len) % MAXSIZE)
		&& this->bitArray.testBit(JSHash((char *)key, len) % MAXSIZE)
		&& this->bitArray.testBit(PJWHash((char *)key, len) % MAXSIZE)
		&& this->bitArray.testBit(SDBMHash((char *)key, len) % MAXSIZE)
		&& this->bitArray.testBit(StrHash((char *)key, len) % MAXSIZE)
		&& this->bitArray.testBit(RSHash((char *)key, len) % MAXSIZE)
		);
}


/*--------------------------
* BloomFilter::addItem
* 	Add an item to the BloomFilter.
* Returns:	bool - Operation result.
* Parameter:
*	void * key - String pointer.
*	int len - Length of string.
----------------------------*/
bool BloomFilter::addItem(void *key, int len)
{
	//Update maxWordLength
	if (len / 2 > this->maxLength)
		this->maxLength = len / 2;

	try
	{
		this->bitArray.setBit(APHash((char *)key, len) % MAXSIZE);
		this->bitArray.setBit(BKDRHash((char *)key, len) % MAXSIZE);
		this->bitArray.setBit(BPHash((char *)key, len) % MAXSIZE);
		this->bitArray.setBit(DEKHash((char *)key, len) % MAXSIZE);
		this->bitArray.setBit(DJBHash((char *)key, len) % MAXSIZE);
		this->bitArray.setBit(ELFHash((char *)key, len) % MAXSIZE);
		this->bitArray.setBit(FNVHash((char *)key, len) % MAXSIZE);
		this->bitArray.setBit(HFHash((char *)key, len) % MAXSIZE);
		this->bitArray.setBit(HFLPHash((char *)key, len) % MAXSIZE);
		this->bitArray.setBit(JSHash((char *)key, len) % MAXSIZE);
		this->bitArray.setBit(PJWHash((char *)key, len) % MAXSIZE);
		this->bitArray.setBit(RSHash((char *)key, len) % MAXSIZE);
		this->bitArray.setBit(SDBMHash((char *)key, len) % MAXSIZE);
		this->bitArray.setBit(StrHash((char *)key, len) % MAXSIZE);
	}
	catch (...)
	{
		return false;
	}
	return true;
}


/*--------------------------
* BloomFilter::getMaxLength
* 	Get the maximum word length of the dictioanry.
* Returns:	unsigned int - Maximum word length.
----------------------------*/
unsigned int BloomFilter::getMaxLength()
{
	return this->maxLength;
}


/*--------------------------
* BloomFilter::hasItem
* 	Override function,provides support for QString.
* Returns:	bool - Result.
* Parameter:
*	const QString & key - The key to search.
----------------------------*/
bool BloomFilter::hasItem(const QString &key)
{
	if (key == "")
		return false;
	QByteArray ba = key.toLocal8Bit();
	char * str = ba.data();
	return this->hasItem(str, ba.size());
}


/*--------------------------
* BloomFilter::addItem
* 	Override function,provides support for QString.
* Returns:	bool - Operation result.
* Parameter:
* 	const QString & key - The key to add.
----------------------------*/
bool BloomFilter::addItem(const QString &key)
{
	if (key == "")
		return false;
	QByteArray ba = key.toLocal8Bit();
	char * str = ba.data();

	return this->addItem(str, ba.size());
}


/*--------------------------
* 14 Hash function list
* Returns:	unsigned int - The hash code.
* Parameter:
* 	char * str - String to hash.
* 	unsigned int len - Length of the string.
----------------------------*/
unsigned int BloomFilter::RSHash(char* str, unsigned int len) {
	unsigned int b = 378551;
	unsigned int a = 63689;
	unsigned int hash = 0;
	unsigned int i = 0;

	for (i = 0; i<len; str++, i++) {
		hash = hash*a + (*str);
		a = a*b;
	}
	return hash;
}


unsigned int BloomFilter::JSHash(char* str, unsigned int len)
{
	unsigned int hash = 1315423911;
	unsigned int i = 0;

	for (i = 0; i<len; str++, i++) {
		hash ^= ((hash << 5) + (*str) + (hash >> 2));
	}
	return hash;
}


unsigned int BloomFilter::PJWHash(char* str, unsigned int len)
{
	const unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);
	const unsigned int ThreeQuarters = (unsigned int)((BitsInUnsignedInt * 3) / 4);
	const unsigned int OneEighth = (unsigned int)(BitsInUnsignedInt / 8);
	const unsigned int HighBits = (unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
	unsigned int hash = 0;
	unsigned int test = 0;
	unsigned int i = 0;

	for (i = 0; i<len; str++, i++) {
		hash = (hash << OneEighth) + (*str);
		if ((test = hash & HighBits) != 0) {
			hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
		}
	}

	return hash;
}


unsigned int BloomFilter::ELFHash(char* str, unsigned int len)
{
	unsigned int hash = 0;
	unsigned int x = 0;
	unsigned int i = 0;

	for (i = 0; i < len; str++, i++) {
		hash = (hash << 4) + (*str);
		if ((x = hash & 0xF0000000L) != 0) {
			hash ^= (x >> 24);
		}
		hash &= ~x;
	}
	return hash;
}


unsigned int BloomFilter::BKDRHash(char* str, unsigned int len)
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


unsigned int BloomFilter::SDBMHash(char* str, unsigned int len)
{
	unsigned int hash = 0;
	unsigned int i = 0;

	for (i = 0; i < len; str++, i++) {
		hash = (*str) + (hash << 6) + (hash << 16) - hash;
	}

	return hash;
}


unsigned int BloomFilter::DJBHash(char* str, unsigned int len)
{
	unsigned int hash = 5381;
	unsigned int i = 0;

	for (i = 0; i < len; str++, i++) {
		hash = ((hash << 5) + hash) + (*str);
	}

	return hash;
}


unsigned int BloomFilter::DEKHash(char* str, unsigned int len)
{
	unsigned int hash = len;
	unsigned int i = 0;

	for (i = 0; i < len; str++, i++) {
		hash = ((hash << 5) ^ (hash >> 27)) ^ (*str);
	}
	return hash;
}


unsigned int BloomFilter::BPHash(char* str, unsigned int len)
{
	unsigned int hash = 0;
	unsigned int i = 0;
	for (i = 0; i < len; str++, i++) {
		hash = hash << 7 ^ (*str);
	}

	return hash;
}


unsigned int BloomFilter::FNVHash(char* str, unsigned int len)
{
	const unsigned int fnv_prime = 0x811C9DC5;
	unsigned int hash = 0;
	unsigned int i = 0;

	for (i = 0; i < len; str++, i++) {
		hash *= fnv_prime;
		hash ^= (*str);
	}

	return hash;
}


unsigned int BloomFilter::APHash(char* str, unsigned int len)
{
	unsigned int hash = 0xAAAAAAAA;
	unsigned int i = 0;

	for (i = 0; i < len; str++, i++) {
		hash ^= ((i & 1) == 0) ? ((hash << 7) ^ (*str) * (hash >> 3)) :
			(~((hash << 11) + (*str) ^ (hash >> 5)));
	}

	return hash;
}


unsigned int BloomFilter::HFLPHash(char *str, unsigned int len)
{
	unsigned int n = 0;
	int i;
	char* b = (char *)&n;
	for (i = 0; i<strlen(str); ++i) {
		b[i % 4] ^= str[i];
	}
	return n%len;
}


unsigned int BloomFilter::HFHash(char* str, unsigned int len)
{
	int result = 0;
	char* ptr = str;
	int c;
	int i = 0;
	for (i = 1; c = *ptr++; i++)
		result += c * 3 * i;
	if (result<0)
		result = -result;
	return result%len;
}


unsigned int BloomFilter::StrHash(char *str, unsigned int len)
{
	register unsigned int   h;
	register unsigned char *p;
	for (h = 0, p = (unsigned char *)str; *p; p++) {
		h = 31 * h + *p;
	}

	return h;

}
/* End of hash function list */
