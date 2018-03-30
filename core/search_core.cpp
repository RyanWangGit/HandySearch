#include "stable.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QtConcurrent>
#include "search_core.h"

SearchCore::SearchCore(const QString &dictionary, const QString &database)
{
    this->setPath(dictionary, database);
    this->hasLoaded = false;
}


SearchCore::SearchCore(const QString &database)
{
    this->setPath(database);
    this->hasLoaded = false;
}


SearchCore::SearchCore()
{
    this->hasLoaded = false;
}


void SearchCore::setPath(const QString &database)
{
    this->dictionaryPath = "qrc:/assets/dictionary.txt";
    this->databasePath = database;
}


void SearchCore::setPath(const QString &dictionary, const QString &database)
{
    this->dictionaryPath = dictionary;
    this->databasePath = database;
}


void SearchCore::query(const QString &sentence)
{
    if(!this->hasLoaded)
        qFatal("Core hasn't loaded anything yet.");

    QStringList keywords;
    QList<Webpage *> webpages;
    // TODO: implement query
    emit this->result(keywords, webpages);
    return;
}


const QString &SearchCore::getDatabasePath() const
{
    return this->databasePath;
}

// used by mapper and reducer since they have to be static functions
static SearchCore *_core = NULL;

QMultiHash<QString, Index *> mapper(const QPair<int, int> &task)
{
    // open a thread-specific database connection
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", QUuid::createUuid().toString());
    db.setDatabaseName(_core->getDatabasePath());
    if(!db.open())
        qFatal("Database cannot be opened.");

    QSqlQuery query(db);
    // note: ROWID implementation only works for SQLITE
    query.prepare("SELECT id, content from `webpages` WHERE ROWID >= :start AND ROWID <= :end");
    query.bindValue(":start", task.first);
    query.bindValue(":end", task.second);
    query.exec();

    QMultiHash<QString, Index *> result;
    while (!query.next()) {
        // TODO: create Index and insert into result hashmap.
    }
    db.close();
    return result;
}


QMultiHash<QString, Index *> &reducer(QMultiHash<QString, Index *> &result, const QMultiHash<QString, Index *> &other)
{
    result.unite(other);
    return result;
}


void SearchCore::load(int from)
{
    this->hasLoaded = true;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", QUuid::createUuid().toString());
    db.setDatabaseName(this->databasePath);
    if(!db.open())
        qFatal("Database cannot be opened.");
    QSqlQuery query("SELECT COUNT(id) FROM `webpages`", db);
    if(!query.next())
        qFatal("Database execution failed.");

    _core = &(*this);

    const int TOTAL_WEBPAGES = query.value(0).toInt();
    const int WEBPAGES_PER_THREAD = float(TOTAL_WEBPAGES - from + 1) / QThread::idealThreadCount();

    db.close();

    QList<QPair<int, int> > tasks;
    for(int i = from; i < TOTAL_WEBPAGES; i += WEBPAGES_PER_THREAD)
    {
        if(i + WEBPAGES_PER_THREAD >= TOTAL_WEBPAGES)
            tasks.append(QPair<int, int>(i, TOTAL_WEBPAGES));
        else
            tasks.append(QPair<int, int>(i, i + WEBPAGES_PER_THREAD - 1));
    }

    this->invertedList = QtConcurrent::blockingMappedReduced<QMultiHash<QString, Index *> >(tasks, mapper, reducer, QtConcurrent::UnorderedReduce);
    qDebug() << this->invertedList;
}
