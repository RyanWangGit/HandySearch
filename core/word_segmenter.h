#pragma once
#include "dictionary.h"


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


class WordSegmenter
{
private:
    QString content;
    int pos;
    const Dictionary *dict;
    void mmFilter(QList<Chunk *> &chunks);
    void lawlFilter(QList<Chunk *> &chunks);
    void svwlFilter(QList<Chunk *> &chunks);
    void sdmfFilter(QList<Chunk *> &chunks);
    bool isChineseChar(const QChar &ch);
    QChar peekNextChar() const;
    QStringList getMaxMatchingWord();
    QStringList getChineseWords();
    QString getASCIIWords();
    void createChunks(QList<Chunk *> &chunks);
public:
    WordSegmenter(const Dictionary *dict);
    QStringList segment(const QString &content);
};

