#include "stdafx.h"

BloomFilter::BloomFilter()
{
	this->bitArray.resize(MAXSIZE);
}


bool BloomFilter::hasItem(void *key, int len)
{
	return
		(
		this->bitArray.testBit(RSHash((char *)key, len) % MAXSIZE)
		&& this->bitArray.testBit(JSHash((char *)key, len) % MAXSIZE)
		&& this->bitArray.testBit(PJWHash((char *)key, len) % MAXSIZE)
		&& this->bitArray.testBit(ELFHash((char *)key, len) % MAXSIZE)
		&& this->bitArray.testBit(BKDRHash((char *)key, len) % MAXSIZE)
		&& this->bitArray.testBit(SDBMHash((char *)key, len) % MAXSIZE)
		&& this->bitArray.testBit(DJBHash((char *)key, len) % MAXSIZE)
		&& this->bitArray.testBit(DEKHash((char *)key, len) % MAXSIZE)
		&& this->bitArray.testBit(BPHash((char *)key, len) % MAXSIZE)
		&& this->bitArray.testBit(FNVHash((char *)key, len) % MAXSIZE)
		&& this->bitArray.testBit(APHash((char *)key, len) % MAXSIZE)
		&& this->bitArray.testBit(HFLPHash((char *)key, len) % MAXSIZE)
		&& this->bitArray.testBit(HFHash((char *)key, len) % MAXSIZE)
		&& this->bitArray.testBit(StrHash((char *)key, len) % MAXSIZE)
		);
}


bool BloomFilter::addItem(void *key, int len)
{
	try
	{
		this->bitArray.setBit(RSHash((char *)key, len) % MAXSIZE);
		this->bitArray.setBit(JSHash((char *)key, len) % MAXSIZE);
		this->bitArray.setBit(PJWHash((char *)key, len) % MAXSIZE);
		this->bitArray.setBit(ELFHash((char *)key, len) % MAXSIZE);
		this->bitArray.setBit(BKDRHash((char *)key, len) % MAXSIZE);
		this->bitArray.setBit(SDBMHash((char *)key, len) % MAXSIZE);
		this->bitArray.setBit(DJBHash((char *)key, len) % MAXSIZE);
		this->bitArray.setBit(DEKHash((char *)key, len) % MAXSIZE);
		this->bitArray.setBit(BPHash((char *)key, len) % MAXSIZE);
		this->bitArray.setBit(FNVHash((char *)key, len) % MAXSIZE);
		this->bitArray.setBit(APHash((char *)key, len) % MAXSIZE);
		this->bitArray.setBit(HFLPHash((char *)key, len) % MAXSIZE);
		this->bitArray.setBit(HFHash((char *)key, len) % MAXSIZE);
		this->bitArray.setBit(StrHash((char *)key, len) % MAXSIZE);
	}
	catch (...)
	{
		return false;
	}
	return true;
}

bool BloomFilter::hasItem(const QString &key)
{
	if (key == "")
		return false;
	QByteArray ba = key.toLocal8Bit();
	char * str = ba.data();
	return this->hasItem(str, ba.size());
}


bool BloomFilter::addItem(const QString &key)
{
	if (key == "")
		return false;
	QByteArray ba = key.toLocal8Bit();
	char * str = ba.data();
	return this->addItem(str, ba.size());
}


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
/* End Of RS Hash Function */


unsigned int BloomFilter::JSHash(char* str, unsigned int len)
{
	unsigned int hash = 1315423911;
	unsigned int i = 0;

	for (i = 0; i<len; str++, i++) {
		hash ^= ((hash << 5) + (*str) + (hash >> 2));
	}
	return hash;
}
/* End Of JS Hash Function */


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
/* End Of  P. J. Weinberger Hash Function */


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
/* End Of ELF Hash Function */


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
/* End Of BKDR Hash Function */


unsigned int BloomFilter::SDBMHash(char* str, unsigned int len)
{
	unsigned int hash = 0;
	unsigned int i = 0;

	for (i = 0; i < len; str++, i++) {
		hash = (*str) + (hash << 6) + (hash << 16) - hash;
	}

	return hash;
}
/* End Of SDBM Hash Function */


unsigned int BloomFilter::DJBHash(char* str, unsigned int len)
{
	unsigned int hash = 5381;
	unsigned int i = 0;

	for (i = 0; i < len; str++, i++) {
		hash = ((hash << 5) + hash) + (*str);
	}

	return hash;
}
/* End Of DJB Hash Function */


unsigned int BloomFilter::DEKHash(char* str, unsigned int len)
{
	unsigned int hash = len;
	unsigned int i = 0;

	for (i = 0; i < len; str++, i++) {
		hash = ((hash << 5) ^ (hash >> 27)) ^ (*str);
	}
	return hash;
}
/* End Of DEK Hash Function */


unsigned int BloomFilter::BPHash(char* str, unsigned int len)
{
	unsigned int hash = 0;
	unsigned int i = 0;
	for (i = 0; i < len; str++, i++) {
		hash = hash << 7 ^ (*str);
	}

	return hash;
}
/* End Of BP Hash Function */


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
/* End Of FNV Hash Function */


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
/* End Of AP Hash Function */


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
/* End Of HFLP Hash Function*/


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
/*End Of HKHash Function */


unsigned int BloomFilter::StrHash(char *str, unsigned int len)
{
	register unsigned int   h;
	register unsigned char *p;
	for (h = 0, p = (unsigned char *)str; *p; p++) {
		h = 31 * h + *p;
	}

	return h;

}
/*End Of StrHash Function*/
