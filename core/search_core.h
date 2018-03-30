#pragma once

#include <QString>
#include <QStringList>
#include <QList>
#include <QObject>
#include <QMultiHash>
#include "webpage.h"
#include "dictionary.h"
#include "inverted_list.h"
#include "index.h"

class SearchCore : public QObject
{
    Q_OBJECT
private:
    Dictionary dictionary;
    bool hasLoaded;
    QString dictionaryPath;
    QString databasePath;
    QMultiHash<QString, Index *> invertedList;
public:
    SearchCore(const QString &dictionary, const QString &database);
    SearchCore(const QString &database);
    SearchCore();
    void setPath(const QString &dictionary, const QString &database);
    void setPath(const QString &database);
    void load(int from = 1);
    void query(const QString &sentence);
signals:
    void result(const QStringList &keywords, const QList<Webpage *> &webpages);
};
