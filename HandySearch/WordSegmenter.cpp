#include "stdafx.h"
#include "WordSegmenter.h"

Chunk::Chunk(QString fWord,QString sWord,QString tWord)
{
	this->words.append(fWord);
	this->words.append(sWord);
	this->words.append(tWord);
	this->wordCount = -1;
	this->totalLen = -1;
	this->avgLen = -1;
	this->variance = -1;
}

Chunk::Chunk()
{
	this->wordCount = -1;
	this->totalLen = -1;
	this->avgLen = -1;
	this->variance = -1;
}

Chunk::Chunk(const Chunk & other)
{
	this->operator=(other);
}

int Chunk::getCount()
{
	if (this->wordCount < 0)
	{
		this->wordCount = 0;
		for (QString word : words)
			if (!word.isEmpty())
				this->wordCount++;
	}
	return this->wordCount;
}

int Chunk::getLength()
{
	if (this->totalLen < 0)
	{
		this->totalLen = 0;
		for (QString word : words)
		{
			if (!word.isEmpty())
				this->totalLen += word.size();
		}
	}
	return this->totalLen;
}

int Chunk::getAvgLen()
{
	if (this->avgLen < 0)
		this->avgLen = (double)this->getLength() / this->getCount();
	return this->avgLen;
}

double Chunk::getVariance()
{
	if (this->variance < 0)
	{
		double sum = 0;
		for (QString word : words)
		{
			if (!word.isEmpty())
			{
				double var = word.size() - this->getAvgLen();
				sum += var * var;
			}
		}
		this->variance = sum / this->getCount();
	}
	return this->variance;
}


QStringList & Chunk::getWords()
{
	return this->words;
}

Chunk & Chunk::operator=(const Chunk &other)
{
	this->words = other.words;

	this->avgLen = other.avgLen;
	this->totalLen = other.totalLen;
	this->variance = other.variance;
	this->wordCount = other.wordCount;
	return *this;
}

void WordSegmenter::mmFilter(List<Chunk> &chunks)
{
	//Filter with segmentation length
	int maxLength = 0;
	for (int i = 0; i < chunks.size(); i++)
		if (chunks.get(i).getLength() > maxLength)
			maxLength = chunks.get(i).getLength();
	//Remove those don't fit 
	for (int i = 0; i < chunks.size(); i++)
		if (chunks.get(i).getLength() < maxLength)
		{
			chunks.remove(i);
			i--;
		}
}

void WordSegmenter::lawlFilter(List<Chunk> &chunks)
{
	//Filter with average word length
	int maxLength = 0;
	for (int i = 0; i < chunks.size(); i++)
		if (chunks.get(i).getAvgLen() > maxLength)
			maxLength = chunks.get(i).getAvgLen();
	//Remove those don't fit 
	for (int i = 0; i < chunks.size(); i++)
		if (chunks.get(i).getAvgLen() < maxLength)
		{
			chunks.remove(i);
			i--;
		}
}

void WordSegmenter::svmlFilter(List<Chunk> &chunks)
{
	//Fiter with variance of word length
	double minVariance = 10000.0;
	for (int i = 0; i < chunks.size(); i++)
		if (chunks.get(i).getVariance() < minVariance)
			minVariance = chunks.get(i).getVariance();
	//Remove those don't fit 
	for (int i = 0; i < chunks.size(); i++)
		if (chunks.get(i).getVariance() < minVariance)
		{
			chunks.remove(i);
			i--;
		}
}

void WordSegmenter::logFreqFilter(List<Chunk> &chunks)
{
	//Due to lack of information
	//I didn't implement this method
	return;
}

bool WordSegmenter::isChineseChar(QChar &ch)
{
	return (ch.unicode() >= 0x4e00 && ch.unicode() <= 0x9FA5);
}


QChar WordSegmenter::getNextChar()
{
	return this->content.data()[this->pos];
}

//Return all possible words segmented using MaxMatching method
QStringList WordSegmenter::getMaxMatchingWord()
{
	QStringList words;
	unsigned int originalPos = this->pos;

	while (this->pos < this->content.size())
	{
		if (this->pos - originalPos > this->dict->getMaxLength())
			break;
		if (!this->isChineseChar(this->getNextChar()))
			break;
		this->pos++;
		
		QString word = this->content.mid(originalPos, this->pos - originalPos);
		if (dict->hasItem(word) || word.size() == 1)
			words.append(word);
	}
	this->pos = originalPos;
	return words;
}

QStringList WordSegmenter::getChineseWords()
{
	//Find all possible chunks
	List<Chunk> chunks;
	this->createChunks(chunks);
	if (chunks.size() > 1)
		this->mmFilter(chunks);
	if (chunks.size() > 1)
		this->lawlFilter(chunks);
	if (chunks.size() > 1)
		this->svmlFilter(chunks);
	if (chunks.size() > 1)
		this->logFreqFilter(chunks);
	if (chunks.size() == 0)
		return QStringList();
	
	//There should be only one chunk remaining
	//after the four filters
	if (chunks.size() == 0)
		return QStringList();
	this->pos += chunks.get(0).getLength();
	return chunks.get(0).getWords();
}

QString WordSegmenter::getASCIIWords()
{
	//Skip spaces and punctuations
	while (this->pos < this->content.size())
	{
		QChar ch = this->getNextChar();
		if (ch.isLetterOrNumber() || this->isChineseChar(ch))
			break;
		this->pos++;
	}

	unsigned int start = this->pos;

	while (this->pos < this->content.size())
	{
		QChar ch = this->getNextChar();
		if (!ch.isLetterOrNumber())
			break;
		this->pos++;
	}
	unsigned int end = this->pos;

	//Skip spaces and punctuations
	while (this->pos < this->content.size())
	{
		QChar ch = this->getNextChar();
		if (ch.isLetterOrNumber() || this->isChineseChar(ch))
			break;
		this->pos++;
	}
	return this->content.mid(start, end - start);
}

void WordSegmenter::createChunks(List<Chunk> &chunks)
{
	unsigned int originalPos = this->pos;
	QStringList words1 = this->getMaxMatchingWord();
	for (QString word1 : words1)
	{
		this->pos += word1.size();
		if (this->pos < this->content.size())
		{
			QStringList words2 = this->getMaxMatchingWord();
			for (QString word2 : words2)
			{
				this->pos += word2.size();
				if (this->pos < this->content.size())
				{
					QStringList words3 = this->getMaxMatchingWord();
					for (QString word3 : words3)
					{
						if (word3.size() == 0)
							chunks.append(Chunk(word1, word2, QString()));
						else
							chunks.append(Chunk(word1, word2, word3));		
					}
				}
				else if (this->pos == this->content.size())
					chunks.append(Chunk(word1, word2, QString()));
				this->pos -= word2.size();
			}
		}
		else if (this->pos == this->content.size())
			chunks.append(Chunk(word1, QString(), QString()));
		this->pos -= word1.size();
	}
	this->pos = originalPos;
}

WordSegmenter::WordSegmenter(QString &content, BloomFilter &dict)
{
	this->content = content;
	this->dict = &dict;
	this->pos = 0;
}

QStringList & WordSegmenter::getResult()
{
	if (!this->result.isEmpty())
		return this->result;
	qDebug() << this->content;

	while (this->pos < this->content.size())
	{
		//If it is a chinese charactor
		if (this->isChineseChar(this->getNextChar()))
		{
 			QStringList word = this->getChineseWords();
			result.append(word);
		}
		else
		{
			QString word = this->getASCIIWords();
			result.append(word);
		}
	}
	return this->result;
}