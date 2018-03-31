#pragma once
#include <QDir>
#include "bloom_filter.h"


class Dictionary : public QObject
{
    Q_OBJECT
private:
    BloomFilter bf;
    QDir dictFolder;
    unsigned int maxLength;
    bool hasLoaded;
public:
    Dictionary();
    void load();
    void setDictFolder(const QDir &dictFolder);
    bool hasItem(const QString &key) const;
    bool addItem(const QString &key);
    unsigned int getMaxLength() const;
};
