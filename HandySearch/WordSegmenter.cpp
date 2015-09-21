#include "stdafx.h"
#include "WordSegmenter.h"

Chunk::Chunk(QString fWord,QString sWord,QString tWord)
{
	this->words.append(fWord);
	if (!sWord.isEmpty())
		this->words.append(sWord);
	if (!tWord.isEmpty())
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

double Chunk::getAvgLen()
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
	double maxLength = 0;
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
		if (chunks.get(i).getVariance() > minVariance)
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
		if (word.size() == 1  || dict->hasItem(word))
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
	unsigned int start = this->pos;

	if (this->getNextChar().isLetterOrNumber())
		while (this->pos < this->content.size())
		{
			QChar ch = this->getNextChar();
			if (ch.isPunct() || ch.isSpace() || this->isChineseChar(ch))
				break;
			else
				this->pos++;
		}
			
	else
		while (this->pos < this->content.size())
		{
			QChar ch = this->getNextChar();
			if (this->isChineseChar(ch) || ch.isLetterOrNumber())
				break;
			else
				this->pos++;
		}

	QString result = this->content.mid(start, this->pos - start);
	return result;
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

			//If there are no words found
			if (words2.isEmpty())
				chunks.append(Chunk(word1, QString::null, QString::null));

			for (QString word2 : words2)
			{
				this->pos += word2.size();
				if (this->pos < this->content.size())
				{
					QStringList words3 = this->getMaxMatchingWord();

					//If there are no words found
					if (words3.isEmpty())
						chunks.append(Chunk(word1, word2, QString::null));

					for (QString word3 : words3)
						chunks.append(Chunk(word1, word2, word3));
				}
				else if (this->pos == this->content.size())
					chunks.append(Chunk(word1, word2, QString::null));

				this->pos -= word2.size();
			}
		}
		else if (this->pos == this->content.size())
			chunks.append(Chunk(word1, QString::null, QString::null));

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

	while (this->pos < this->content.size())
	{
		//If it is a chinese charactor
		if (this->isChineseChar(this->getNextChar()))
			result.append(this->getChineseWords());
		else
			result.append(this->getASCIIWords());
	}
	return this->result;
}