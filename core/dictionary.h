#pragma once
#include "stable.h"
#include "bloom_filter.h"


class Dictionary : public QObject
{
    Q_OBJECT
private:
    BloomFilter bf;
    unsigned int maxLength;
    bool hasLoaded;
public:
    Dictionary();
    void load(const QString &path);
    bool hasItem(const QString &key) const;
    bool addItem(const QString &key);
    unsigned int getMaxLength() const;
};
