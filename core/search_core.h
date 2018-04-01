#pragma once

#include <QString>
#include <QStringList>
#include <QList>
#include <QObject>
#include <QHash>
#include <QSqlDatabase>
#include <QSharedPointer>
#include "dictionary.h"

class SearchCore : public QObject
{
    Q_OBJECT
private:
    Dictionary dictionary;
    bool hasLoaded;
    QString dictionaryPath, databasePath;
    QHash<QString, QHash<int, QList<int> > > invertedList;
    QSqlDatabase db;
    unsigned int maxProgress;
    unsigned int webpagesCount;
public:
    typedef struct _webpage {
        QString title, brief, url;
    } Webpage;
    SearchCore(const QString &dictionary, const QString &database);
    SearchCore(const QString &database);
    SearchCore();
    ~SearchCore();
    void setPath(const QString &dictionary, const QString &database);
    void setPath(const QString &database);
    const QString &getDatabasePath() const;
    const Dictionary &getDictionary() const;
    unsigned int getMaxProgress() const;
    unsigned int getWebpagesCount() const;
    void load(int from = 1);
    void query(const QString &sentence);
signals:
    void result(const QStringList &keywords, const QList<QSharedPointer<Webpage> > &webpages);
    void progress(const QString &hint, int progress);
};
