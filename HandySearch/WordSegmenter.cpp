/*************************************
 * Copyright(C),2015-2016,Ryan Wang 
 * 
 * File:	WordSegmenter.cpp
 *
 * Version: V1.0
 * 
 * Brief:	This implements a word segementer based on MMSeg algorithm,
 * providing fast segmentation and also real good accuracy.
 *
 * Author:	Ryan
 
 * Date:	Oct. 2015
*************************************/
#include "stdafx.h"
#include "BloomFilter.h"
#include "WordSegmenter.h"
#include "List.h"


/*--------------------------
* Chunk::Chunk
* 	Chunk constructor,construct Chunk object with three words,
* use QString::null to represent empty string,common-used.
* Parameter:
* 	QString fWord - First word.
* 	QString sWord - Second word.
* 	QString tWord - Third word.
----------------------------*/
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


/*--------------------------
* Chunk::Chunk
* 	Default constructor,used for list head node.
----------------------------*/
Chunk::Chunk()
{
	this->wordCount = -1;
	this->totalLen = -1;
	this->avgLen = -1;
	this->variance = -1;
}


/*--------------------------
* Chunk::Chunk
* 	Copy constructor.
* Parameter:
* 	const Chunk & other - The other Chunk object.
----------------------------*/
Chunk::Chunk(const Chunk & other)
{
	this->operator=(other);
}


/*--------------------------
* Chunk::getCount
* 	Returns the number of words the chunk posseses.
* Returns:	int - The number of words.
----------------------------*/
int Chunk::getCount()
{
	if (wordCount < 0)
	{
		wordCount = 0;
		for (QString word : words)
			if (!word.isEmpty())
				wordCount++;
	}
	return wordCount;
}


/*--------------------------
* Chunk::getLength
* 	Returns the total word length of the chunk.
* Returns:	int - The total words length.
----------------------------*/
int Chunk::getLength()
{
	if (totalLen < 0)
	{
		totalLen = 0;
		for (QString word : words)
		{
			if (!word.isEmpty())
				totalLen += word.size();
		}
	}
	return totalLen;
}


/*--------------------------
* Chunk::getAvgLen
* 	Returns the average word length of the chunk.
* Returns:	double - Average word length.
----------------------------*/
double Chunk::getAvgLen()
{
	if (avgLen < 0)
		avgLen = (double)getLength() / getCount();
	return avgLen;
}


/*--------------------------
* Chunk::getVariance
* 	Returns the variance of the chunk.
* Returns:	double - Variance of the chunk.
----------------------------*/
double Chunk::getVariance()
{
	if (variance < 0)
	{
		double sum = 0;
		for (QString word : words)
		{
			if (!word.isEmpty())
			{
				double var = word.size() - getAvgLen();
				sum += var * var;
			}
		}
		variance = sum / getCount();
	}
	return variance;
}


/*--------------------------
* Chunk::getWords
* 	Returns the word list of the chunk.
* Returns:	QStringList & - 
----------------------------*/
QStringList & Chunk::getWords()
{
	return words;
}


/*--------------------------
* Chunk::operator=
* 	Override operator method to assign value directly.
* Returns:	Chunk & - The chunk obejct itself.
* Parameter:
* 	const Chunk & other - The other chunk object.
----------------------------*/
Chunk & Chunk::operator=(const Chunk &other)
{
	this->words = other.words;

	this->avgLen = other.avgLen;
	this->totalLen = other.totalLen;
	this->variance = other.variance;
	this->wordCount = other.wordCount;
	return *this;
}


/*--------------------------
* WordSegmenter::mmFilter
* 	Filter the chunk list with Max-Matching rule.
* Parameter:
* 	List<Chunk> & chunks - Chunk list to be filtered.
----------------------------*/
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


/*--------------------------
* WordSegmenter::lawlFilter
* 	Filter the chunk list with Largest-Average-Word-Length rule.
* Parameter:
* 	List<Chunk> & chunks - Chunk list to be filtered.
----------------------------*/
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


/*--------------------------
* WordSegmenter::svwlFilter
* 	Filter the chunk list with Smallest-Variance-of-Word-Length rule.
* Parameter:
* 	List<Chunk> & chunks - Chunk list to be filtered.
----------------------------*/
void WordSegmenter::svwlFilter(List<Chunk> &chunks)
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


/*--------------------------
* WordSegmenter::logFreqFilter
* 	Filter the chunk list with largest-Sum-of-Degree-of-Morphemic
* -Freedom-of-one-character-words rule.
* Parameter:
* 	List<Chunk> & chunks - Chunk list to be filtered.
----------------------------*/
void WordSegmenter::sdmfFilter(List<Chunk> &chunks)
{
	//Due to lack of information
	//I didn't implement this method
	return;
}


/*--------------------------
* WordSegmenter::isChineseChar
* 	Determine whether a character is Chinese or not.
* Returns:	bool - Result.
* Parameter:
* 	QChar & ch - The character
----------------------------*/
bool WordSegmenter::isChineseChar(QChar &ch)
{
	return (ch.unicode() >= 0x4e00 && ch.unicode() <= 0x9FA5);
}


/*--------------------------
* WordSegmenter::getNextChar
* 	Return the next character of the current position of the content
* without pushing the pos indicator.
* Returns:	QChar - The next character.
----------------------------*/
QChar WordSegmenter::getNextChar()
{
	return content.data()[pos];
}


/*--------------------------
* WordSegmenter::getMaxMatchingWord
* 	Return all possible segmentation results using MaxMatching method.
* Returns:	QStringList - All possible segmentation results.
----------------------------*/
QStringList WordSegmenter::getMaxMatchingWord()
{
	QStringList words;
	unsigned int originalPos = pos;

	while (pos < content.size())
	{
		if (pos - originalPos > dict->getMaxLength())
			break;
		if (!isChineseChar(getNextChar()))
			break;
		pos++;
		
		QString word = content.mid(originalPos, pos - originalPos);
		if (word.size() == 1  || dict->hasItem(word))
			words.append(word);
	}
	pos = originalPos;
	return words;
}


/*--------------------------
* WordSegmenter::getChineseWords
* 	Returns the next three segmented Chinese words(a chunk).
* Returns:	QStringList - Segmented chunk(Chinese word list).
----------------------------*/
QStringList WordSegmenter::getChineseWords()
{
	//Find all possible chunks
	List<Chunk> chunks;
	createChunks(chunks);
	if (chunks.size() > 1)
		mmFilter(chunks);
	if (chunks.size() > 1)
		lawlFilter(chunks);
	if (chunks.size() > 1)
		svwlFilter(chunks);
	if (chunks.size() > 1)
		sdmfFilter(chunks);
	
	//There should be only one chunk remaining
	//after the four filters
	if (chunks.size() == 0)
		return QStringList();

	pos += chunks.get(0).getLength();
	return chunks.get(0).getWords();
}


/*--------------------------
* WordSegmenter::getASCIIWords
* 	Returns the next english word or punctuation.
* Returns:	QString - English word or punctuation.
----------------------------*/
QString WordSegmenter::getASCIIWords()
{
	unsigned int start = pos;

	if (getNextChar().isLetterOrNumber())
		while (pos < content.size())
		{
			QChar ch = getNextChar();
			if (ch.isPunct() || ch.isSpace() || isChineseChar(ch))
				break;
			else
				pos++;
		}
			
	else
		while (pos < content.size())
		{
			QChar ch = getNextChar();
			if (isChineseChar(ch) || ch.isLetterOrNumber())
				break;
			else
				pos++;
		}

	QString result = content.mid(start, pos - start);
	return result;
}


/*--------------------------
* WordSegmenter::createChunks
* 	Create multiple possible chunks.
* Parameter:
* 	List<Chunk> & chunks - List of possible chunks.
----------------------------*/
void WordSegmenter::createChunks(List<Chunk> &chunks)
{
	unsigned int originalPos = pos;
	QStringList words1 = getMaxMatchingWord();
	for (QString word1 : words1)
	{
		pos += word1.size();

		if (pos < content.size())
		{
			QStringList words2 = getMaxMatchingWord();

			//If there are no words found
			if (words2.isEmpty())
				chunks.append(Chunk(word1, QString::null, QString::null));

			for (QString word2 : words2)
			{
				pos += word2.size();
				if (pos < content.size())
				{
					QStringList words3 = getMaxMatchingWord();

					//If there are no words found
					if (words3.isEmpty())
						chunks.append(Chunk(word1, word2, QString::null));

					for (QString word3 : words3)
						chunks.append(Chunk(word1, word2, word3));
				}
				else if (pos == content.size())
					chunks.append(Chunk(word1, word2, QString::null));

				pos -= word2.size();
			}
		}
		else if (pos == content.size())
			chunks.append(Chunk(word1, QString::null, QString::null));

		pos -= word1.size();
	}
	pos = originalPos;
}


/*--------------------------
* WordSegmenter::WordSegmenter
* 	Constructor of segmenter.
* Parameter:
* 	QString & content - The content needed to be segmented.
* 	BloomFilter & dict - Dictionary object.
----------------------------*/
WordSegmenter::WordSegmenter(QString &content, BloomFilter &dict)
{
	this->content = content;
	this->dict = &dict;
	this->pos = 0;
}


/*--------------------------
* WordSegmenter::getResult
* 	Returns the segmented word list as result.
* Returns:	QStringList & - Segmented word list.
----------------------------*/
QStringList & WordSegmenter::getResult()
{
	if (!result.isEmpty())
		return result;

	while (pos < content.size())
	{
		//If it is a chinese charactor
		if (isChineseChar(getNextChar()))
			result.append(getChineseWords());
		else
			result.append(getASCIIWords());
	}
	return result;
}