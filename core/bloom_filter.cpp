/*************************************
 * Copyright(C),2015-2016,Ryan Wang 
 * 
 * File:    BloomFilter.cpp
 *
 * Version: V1.0
 * 
 * Brief:    This is the implementation of BloomFilter,simple use addItem to put
 * new dictionary entry to the dictionary and use hasItem to test whether an entry
 * is in the dictionary or not.
 *
 * Author:    Ryan
 
 * Date:    Oct. 2015
*************************************/
#include "stdafx.h"
#include "bloom_filter.h"


/*--------------------------
* BloomFilter::BloomFilter
*     The default constructor of BloomFilter.
----------------------------*/
BloomFilter::BloomFilter()
{
    this->bitArray.resize(MAXSIZE);
}



/*--------------------------
* BloomFilter::hasItem
*     Find whether the item is in the BloomFilter or not.
* Returns:    bool - Result.
* Parameter:
*    void * key - The key to search.
*    int len - Length of string.
----------------------------*/
bool BloomFilter::hasItem(const void *key, int len) const
{
    return
        (
        bitArray.testBit(APHash((char *)key, len) % MAXSIZE)
        && bitArray.testBit(BKDRHash((char *)key, len) % MAXSIZE)
        && bitArray.testBit(BPHash((char *)key, len) % MAXSIZE)
        && bitArray.testBit(DEKHash((char *)key, len) % MAXSIZE)
        && bitArray.testBit(DJBHash((char *)key, len) % MAXSIZE)
        && bitArray.testBit(ELFHash((char *)key, len) % MAXSIZE)
        && bitArray.testBit(FNVHash((char *)key, len) % MAXSIZE)
        && bitArray.testBit(HFHash((char *)key, len) % MAXSIZE)
        && bitArray.testBit(HFLPHash((char *)key, len) % MAXSIZE)
        && bitArray.testBit(JSHash((char *)key, len) % MAXSIZE)
        && bitArray.testBit(PJWHash((char *)key, len) % MAXSIZE)
        && bitArray.testBit(SDBMHash((char *)key, len) % MAXSIZE)
        && bitArray.testBit(StrHash((char *)key, len) % MAXSIZE)
        && bitArray.testBit(RSHash((char *)key, len) % MAXSIZE)
        );
}


/*--------------------------
* BloomFilter::addItem
*     Add an item to the BloomFilter.
* Returns:    bool - Operation result.
* Parameter:
*    void * key - String pointer.
*    int len - Length of string.
----------------------------*/
bool BloomFilter::addItem(const void *key, int len)
{
    bitArray.setBit(APHash((char *)key, len) % MAXSIZE);
    bitArray.setBit(BKDRHash((char *)key, len) % MAXSIZE);
    bitArray.setBit(BPHash((char *)key, len) % MAXSIZE);
    bitArray.setBit(DEKHash((char *)key, len) % MAXSIZE);
    bitArray.setBit(DJBHash((char *)key, len) % MAXSIZE);
    bitArray.setBit(ELFHash((char *)key, len) % MAXSIZE);
    bitArray.setBit(FNVHash((char *)key, len) % MAXSIZE);
    bitArray.setBit(HFHash((char *)key, len) % MAXSIZE);
    bitArray.setBit(HFLPHash((char *)key, len) % MAXSIZE);
    bitArray.setBit(JSHash((char *)key, len) % MAXSIZE);
    bitArray.setBit(PJWHash((char *)key, len) % MAXSIZE);
    bitArray.setBit(RSHash((char *)key, len) % MAXSIZE);
    bitArray.setBit(SDBMHash((char *)key, len) % MAXSIZE);
    bitArray.setBit(StrHash((char *)key, len) % MAXSIZE);
    return true;
}


/*--------------------------
* 14 Hash function list
* Returns:    unsigned int - The hash code.
* Parameter:
*     char * str - String to hash.
*     unsigned int len - Length of the string.
----------------------------*/
unsigned int BloomFilter::RSHash(const char* str, unsigned int len) const {
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


unsigned int BloomFilter::JSHash(const char* str, unsigned int len) const
{
    unsigned int hash = 1315423911;
    unsigned int i = 0;

    for (i = 0; i<len; str++, i++) {
        hash ^= ((hash << 5) + (*str) + (hash >> 2));
    }
    return hash;
}


unsigned int BloomFilter::PJWHash(const char* str, unsigned int len) const
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


unsigned int BloomFilter::ELFHash(const char* str, unsigned int len) const
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


unsigned int BloomFilter::BKDRHash(const char* str, unsigned int len) const
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


unsigned int BloomFilter::SDBMHash(const char* str, unsigned int len) const
{
    unsigned int hash = 0;
    unsigned int i = 0;

    for (i = 0; i < len; str++, i++) {
        hash = (*str) + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}


unsigned int BloomFilter::DJBHash(const char* str, unsigned int len) const
{
    unsigned int hash = 5381;
    unsigned int i = 0;

    for (i = 0; i < len; str++, i++) {
        hash = ((hash << 5) + hash) + (*str);
    }

    return hash;
}


unsigned int BloomFilter::DEKHash(const char* str, unsigned int len) const
{
    unsigned int hash = len;
    unsigned int i = 0;

    for (i = 0; i < len; str++, i++) {
        hash = ((hash << 5) ^ (hash >> 27)) ^ (*str);
    }
    return hash;
}


unsigned int BloomFilter::BPHash(const char* str, unsigned int len) const
{
    unsigned int hash = 0;
    unsigned int i = 0;
    for (i = 0; i < len; str++, i++) {
        hash = hash << 7 ^ (*str);
    }

    return hash;
}


unsigned int BloomFilter::FNVHash(const char* str, unsigned int len) const
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


unsigned int BloomFilter::APHash(const char* str, unsigned int len) const
{
    unsigned int hash = 0xAAAAAAAA;
    unsigned int i = 0;

    for (i = 0; i < len; str++, i++) {
        hash ^= ((i & 1) == 0) ? ((hash << 7) ^ (*str) * (hash >> 3)) :
            (~((hash << 11) + (*str) ^ (hash >> 5)));
    }

    return hash;
}


unsigned int BloomFilter::HFLPHash(const char* str, unsigned int len) const
{
    unsigned int n = 0;
    size_t i;
    char* b = (char *)&n;
    for (i = 0; i<strlen(str); ++i) {
        b[i % 4] ^= str[i];
    }
    return n%len;
}


unsigned int BloomFilter::HFHash(const char* str, unsigned int len) const
{
    int result = 0;
    int c;
    int i = 0;
    for (i = 1; (c = *str++); i++)
        result += c * 3 * i;
    if (result<0)
        result = -result;
    return result%len;
}


unsigned int BloomFilter::StrHash(const char* str, unsigned int) const
{
    unsigned int   h;
    unsigned char *p;
    for (h = 0, p = (unsigned char *)str; *p; p++) {
        h = 31 * h + *p;
    }

    return h;

}
/* End of hash function list */
