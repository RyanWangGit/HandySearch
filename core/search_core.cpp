#include "stable.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QtConcurrent>
#include "search_core.h"
#include "word_segmenter.h"

// < webpage_id, positions >
typedef QPair<int, QList<int> > Index;
// < word, index >
typedef QMultiHash<QString, Index > InvertedList;

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

SearchCore::~SearchCore()
{
    this->db.close();
}


void SearchCore::setPath(const QString &database)
{
    this->dictionaryPath = ":/assets/dictionary.txt";
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

    WordSegmenter ws = WordSegmenter(&this->dictionary);
    QStringList keywords = ws.segment(sentence);
    QList<QSharedPointer<Webpage> > webpages;

    QSqlQuery query(this->db);
    for(QString & word : keywords)
    {
        QSharedPointer<Webpage> webpage = QSharedPointer<Webpage>(new Webpage);
        for(Index & index : this->invertedList.values(word))
        {
            query.prepare("SELECT title, content, url from `webpages` WHERE id == :id");
            query.bindValue(":id", index.first);
            if(!query.exec() || !query.next())
                qFatal(query.lastError().text().toLatin1().data());

            webpage->title = query.value(0).toString();
            // TODO: find the best-fit brief
            webpage->brief = query.value(1).toString().mid(0, 30);
            webpage->url = query.value(2).toString();
            query.clear();
        }
        webpages.append(webpage);
    }

    // TODO: evaluate and order the results
    emit this->result(keywords, webpages);
    return;
}


const QString &SearchCore::getDatabasePath() const
{
    return this->databasePath;
}

const Dictionary &SearchCore::getDictionary() const
{
    return this->dictionary;
}


// used by mapper and reducer since they have to be static functions
static SearchCore *_core = NULL;

// helper function
inline void putInInvertedList(const QString &word, int pos, int id, InvertedList &result)
{
    bool hasFound = false;
    if(result.contains(word))
    {
        for(Index &index : result.values(word))
        {
            // found existing index
            if(index.first == id)
            {
                hasFound = true;
                index.second.append(pos);
                break;
            }
        }
    }

    // create index and insert
    if(!hasFound)
    {
        Index index;
        index.first = id;
        index.second.append(pos);
        result.insert(word, index);
    }
}

InvertedList mapper(const QPair<int, int> &task)
{
    // open a thread-specific database connection
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", QUuid::createUuid().toString());
    db.setDatabaseName(_core->getDatabasePath());
    if(!db.open())
        qFatal("Database cannot be opened.");

    // note: ROWID implementation only works for SQLITE
    QSqlQuery query(db);
    query.prepare("SELECT id, title, content from `webpages` WHERE ROWID >= :start AND ROWID <= :end");
    query.bindValue(":start", task.first);
    query.bindValue(":end", task.second);
    if(!query.exec())
        qFatal(query.lastError().text().toLatin1().data());

    WordSegmenter ws(&_core->getDictionary());

    InvertedList result;

    unsigned int i;
    for (i = 0; query.next(); i++)
    {
        int id = query.value(0).toInt();

        int pos = 0;
        for(QString & word : ws.segment(query.value(1).toString()))
        {
            pos -= word.size();
            putInInvertedList(word, pos, id, result);
        }

        pos = 0;
        for(QString & word : ws.segment(query.value(2).toString()))
        {
            pos += word.size();
            putInInvertedList(word, pos, id, result);
        }
    }
    db.close();

    return result;
}


InvertedList &reducer(InvertedList &result, const InvertedList &other)
{
    result.unite(other);
    return result;
}


void SearchCore::load(int from)
{
    // load dictonary
    this->dictionary.load(this->dictionaryPath);

    // load webpages

    // open database
    this->db = QSqlDatabase::addDatabase("QSQLITE", QUuid::createUuid().toString());
    this->db.setDatabaseName(this->databasePath);
    if(!this->db.open())
        qFatal("Database cannot be opened.");
    QSqlQuery query("SELECT COUNT(id) FROM `webpages`", this->db);
    if(!query.next())
        qFatal("Database execution failed.");

    // assign the workload
    const int TOTAL_WEBPAGES = query.value(0).toInt();
    const int WEBPAGES_PER_THREAD = float(TOTAL_WEBPAGES - from + 1) / QThread::idealThreadCount();

    QList<QPair<int, int> > tasks;
    for(int i = from; i < TOTAL_WEBPAGES; i += WEBPAGES_PER_THREAD)
    {
        if(i + WEBPAGES_PER_THREAD >= TOTAL_WEBPAGES)
            tasks.append(QPair<int, int>(i, TOTAL_WEBPAGES));
        else
            tasks.append(QPair<int, int>(i, i + WEBPAGES_PER_THREAD - 1));
    }

    // set static global variable to be used in mapper and reducer
    _core = &(*this);

    // mapreduce to process all the webpages
    this->invertedList = QtConcurrent::blockingMappedReduced<InvertedList>(tasks, mapper, reducer, QtConcurrent::UnorderedReduce);

    this->hasLoaded = true;
}
