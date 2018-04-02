#include "stable.h"
#include "word_segmenter.h"
#include "bloom_filter.h"


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
    if (wordCount < 0)
    {
        wordCount = 0;
        for (QString word : words)
            if (!word.isEmpty())
                wordCount++;
    }
    return wordCount;
}


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


double Chunk::getAvgLen()
{
    if (avgLen < 0)
        avgLen = (double)getLength() / getCount();
    return avgLen;
}


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


QStringList & Chunk::getWords()
{
    return words;
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


void WordSegmenter::mmFilter(QList<Chunk *> &chunks)
{
    //Filter with segmentation length
    int maxLength = 0;
    for(Chunk * chunk : chunks)
        if(chunk->getLength() > maxLength)
            maxLength = chunk->getLength();

    //Remove those don't fit 
    for (int i = 0; i < chunks.size(); i++)
        if (chunks.at(i)->getLength() < maxLength)
        {
            delete chunks.takeAt(i);
            i--;
        }
}


void WordSegmenter::lawlFilter(QList<Chunk *> &chunks)
{
    //Filter with average word length
    double maxLength = 0;
    for (Chunk * chunk : chunks)
        if (chunk->getAvgLen() > maxLength)
            maxLength = chunk->getAvgLen();

    //Remove those don't fit 
    for (int i = 0; i < chunks.size(); i++)
        if (chunks.at(i)->getAvgLen() < maxLength)
        {
            delete chunks.takeAt(i);
            i--;
        }
}


void WordSegmenter::svwlFilter(QList<Chunk *> &chunks)
{
    //Fiter with variance of word length
    double minVariance = 10000.0;
    for (Chunk * chunk : chunks)
        if (chunk->getVariance() < minVariance)
            minVariance = chunk->getVariance();
    //Remove those don't fit 
    for (int i = 0; i < chunks.size(); i++)
        if (chunks.at(i)->getVariance() > minVariance)
        {
            delete chunks.takeAt(i);
            i--;
        }
}


void WordSegmenter::sdmfFilter(QList<Chunk *> &chunks)
{
    //Due to lack of information
    //I didn't implement this method
    return;
}


bool WordSegmenter::isChineseChar(const QChar &ch)
{
    return (ch.unicode() >= 0x4e00 && ch.unicode() <= 0x9FA5);
}


QChar WordSegmenter::peekNextChar() const
{
    return content.data()[pos];
}


QStringList WordSegmenter::getMaxMatchingWord()
{
    QStringList words;
    unsigned int originalPos = pos;

    while (pos < content.size())
    {
        if (pos - originalPos > dict->getMaxLength())
            break;
        if (!isChineseChar(peekNextChar()))
            break;
        pos++;
        
        QString word = content.mid(originalPos, pos - originalPos);
        if (word.size() == 1  || dict->hasItem(word))
            words.append(word);
    }
    pos = originalPos;
    return words;
}


QStringList WordSegmenter::getChineseWords()
{
    //Find all possible chunks
    QList<Chunk *> chunks;
    createChunks(chunks);
    if (chunks.size() > 1)
        mmFilter(chunks);
    if (chunks.size() > 1)
        lawlFilter(chunks);
    if (chunks.size() > 1)
        svwlFilter(chunks);
    if (chunks.size() > 1)
        sdmfFilter(chunks);
    
    // There should be only one chunk remaining
    //after the four filters
    if (chunks.size() == 0)
        return QStringList();

    pos += chunks.at(0)->getLength();
    QStringList result = chunks.at(0)->getWords();

    // release memory
    for(Chunk * chunk : chunks)
        delete chunk;

    return result;
}


QString WordSegmenter::getASCIIWords()
{
    unsigned int start = pos;

    if (peekNextChar().isLetterOrNumber())
        while (pos < content.size())
        {
            QChar ch = peekNextChar();
            if (ch.isPunct() || ch.isSpace() || isChineseChar(ch))
                break;
            else
                pos++;
        }
            
    else
        while (pos < content.size())
        {
            QChar ch = peekNextChar();
            if (isChineseChar(ch) || ch.isLetterOrNumber())
                break;
            else
                pos++;
        }

    QString result = content.mid(start, pos - start);
    return result;
}


void WordSegmenter::createChunks(QList<Chunk *> &chunks)
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
                chunks.append(new Chunk(word1, QString::null, QString::null));

            for (QString word2 : words2)
            {
                pos += word2.size();
                if (pos < content.size())
                {
                    QStringList words3 = getMaxMatchingWord();

                    //If there are no words found
                    if (words3.isEmpty())
                        chunks.append(new Chunk(word1, word2, QString::null));

                    for (QString word3 : words3)
                        chunks.append(new Chunk(word1, word2, word3));
                }
                else if (pos == content.size())
                    chunks.append(new Chunk(word1, word2, QString::null));

                pos -= word2.size();
            }
        }
        else if (pos == content.size())
            chunks.append(new Chunk(word1, QString::null, QString::null));

        pos -= word1.size();
    }
    pos = originalPos;
}


WordSegmenter::WordSegmenter(const Dictionary *dict)
    :dict(dict)
{
    this->pos = 0;
}


QStringList WordSegmenter::segment(const QString &content)
{
    QStringList result;
    this->content = content;
    this->pos = 0;

    while (pos < content.size())
    {
        //If it is a chinese charactor
        if (isChineseChar(peekNextChar()))
            result.append(getChineseWords());
        else
            result.append(getASCIIWords());
    }
    return result;
}
