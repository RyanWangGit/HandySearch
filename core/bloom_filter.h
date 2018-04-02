#pragma once
#include <QBitArray>


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
