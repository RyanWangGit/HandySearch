#pragma once

#include <QString>
#include <QStringList>
#include <QList>
#include <QObject>
#include "webpage.h"
#include "dictionary.h"
#include "inverted_list.h"

class SearchCore
{
    Q_OBJECT
private:
    Dictionary dictionary;
    InvertedList invertedList;
    bool hasLoaded;
public:
    SearchCore(const QString &dictionary, const QString &database);
    void load();
    void query(const QString &sentence);
signals:
    void result(const QStringList &keywords, const QList<Webpage *> &webpages);
};
