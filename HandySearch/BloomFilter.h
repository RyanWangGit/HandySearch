
#pragma once

class BloomFilter
{
#define MAXSIZE 67108864 // 2^26
private:
	QBitArray bitArray;
	unsigned int maxLength;
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
	bool hasItem(const QString &key);
	bool addItem(const QString &key);
	unsigned int getMaxLength();
};
