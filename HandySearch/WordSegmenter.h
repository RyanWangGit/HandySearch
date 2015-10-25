#pragma once
#include "List.h"
#include "BloomFilter.h"
#include "Html.h"


/**
 * Class:	Chunk
 *
 * Brief:	A chunk consists of at most three word,it represents 
 * one possible segmentation.
 *
 * Date:	Oct. 2015
 */
class Chunk
{
private:
	QStringList words;
	int wordCount;
	int totalLen;
	double avgLen;
	double variance;
public:
	Chunk();
	Chunk(const Chunk & other);
	Chunk(QString fWord, QString sWord, QString tWord);
	int getCount();
	int getLength();
	double getAvgLen();
	double getVariance();
	QStringList &getWords();
	Chunk &operator=(const Chunk &other);
};


/**
 * Class:	WordSegmenter
 *
 * Brief:	Using MMSeg algorithm,thi class is implemented to 
 * do word segmentation.
 *
 * Date:	Oct. 2015
 */
class WordSegmenter
{
private:
	QString content;
	QStringList result;
	BloomFilter *dict;
	unsigned int pos;
	void mmFilter(List<Chunk> &chunks);
	void lawlFilter(List<Chunk> &chunks);
	void svwlFilter(List<Chunk> &chunks);
	void sdmfFilter(List<Chunk> &chunks);
	bool isChineseChar(QChar &ch);
	bool isPunctuation(QChar *ch);
	QChar getNextChar();
	QStringList getMaxMatchingWord();
	QStringList getChineseWords();
	QString getASCIIWords();
	void createChunks(List<Chunk> &chunks);
public:
	WordSegmenter(QString &content,BloomFilter &dict);
	QStringList & getResult();
};

