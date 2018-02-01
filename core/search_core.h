#pragma once

#include <QString>
#include <QStringList>
#include <QList>
#include <QSharedPointer>
#include "webpage.h"
#include "dictionary.h"
#include "inverted_list.h"

typedef struct SearchResult {
    QStringList keywords;
    QList<Webpage *> webpages;
}SearchResult;

class SearchCore
{
private:
    Dictionary dictionary;
    InvertedList invertedList;
    bool hasLoaded;
public:
    SearchCore(const QString &dictionary, const QString &database);
    void load();
    QSharedPointer<SearchResult> query(const QString &sentence);
};
