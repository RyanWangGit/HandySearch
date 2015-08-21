#pragma once
class Chunk
{
private:
	QStringList words;
	int wordCount;
	int totalLen;
	int avgLen;
	double variance;
public:
	Chunk();
	Chunk(const Chunk & other);
	Chunk(QString fWord, QString sWord, QString tWord);
	int getCount();
	int getLength();
	int getAvgLen();
	double getVariance();
	QStringList &getWords();
	Chunk &operator=(const Chunk &other);
};


class WordSegmenter
{
private:
	QString content;
	QStringList result;
	BloomFilter *dict;
	unsigned int pos;
	void mmFilter(List<Chunk> &chunks);
	void lawlFilter(List<Chunk> &chunks);
	void svmlFilter(List<Chunk> &chunks);
	void logFreqFilter(List<Chunk> &chunks);
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

