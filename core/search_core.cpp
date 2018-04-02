#include "stable.h"
#include <tuple>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QtConcurrent>
#include "search_core.h"
#include "word_segmenter.h"

// <id, positions>
typedef QHash<int, QList<int> > Index;
// < word, index >
typedef QHash<QString, Index> InvertedList;

SearchCore::SearchCore(const QString &dictionary, const QString &database)
{
    this->setPath(dictionary, database);
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


void SearchCore::setPath(const QString &dictionary, const QString &database)
{
    this->dictionaryPath = dictionary;
    this->databasePath = database;
}


const QString &SearchCore::getDatabasePath() const
{
    return this->databasePath;
}

const Dictionary &SearchCore::getDictionary() const
{
    return this->dictionary;
}

unsigned int SearchCore::getWebpagesCount() const
{
    return this->webpagesCount;
}

unsigned int SearchCore::getMaxProgress() const
{
    return this->maxProgress;
}

QStringList SearchCore::getTitleList() const
{
    QSqlQuery query("SELECT title FROM `webpages`", this->db);
    QStringList list;
    while(query.next())
        list.append(query.value(0).toString());
    return list;
}

/* TODO: need more elegant solution */
// used by mapper and reducer since they have to be static functions
static SearchCore *_core = NULL;
// used by mapper and reducer to report progress
static int WEBPAGES_PER_THREAD = -1;

// extract word from webpage and store into <word, id, pos> to be processed in reducer
QList<std::tuple<QString, int, int> > mapper(const QPair<int, int> &task)
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

    QList<std::tuple<QString, int, int> > indexList;

    const int PROGRESS_FREQUENCY = 100;
    unsigned int i;
    for (i = 0; query.next(); i++)
    {
        int id = query.value(0).toInt();

        int pos = 0;
        for(const QString & word : ws.segment(query.value(1).toString()))
        {
            pos -= word.size();
            indexList.append(std::make_tuple(word, id, pos));
        }

        pos = 0;
        for(const QString & word : ws.segment(query.value(2).toString()))
        {
            pos += word.size();
            indexList.append(std::make_tuple(word, id, pos));
        }

        if(i % PROGRESS_FREQUENCY == 0 && i != 0)
            _core->progress("Loading Webpages", PROGRESS_FREQUENCY);

    }

    _core->progress("Loading Webpages", i % PROGRESS_FREQUENCY);
    db.close();

    return indexList;
}


// get <word, id, pos> from mapper and store into the inverted list
void reducer(InvertedList &result, const QList<std::tuple<QString, int, int> > &other)
{
    if(result.isEmpty())
        result.reserve(_core->getWebpagesCount());

    for(const std::tuple<QString, int, int> & index : other)
    {
        const QString &word = std::get<0>(index);
        int id = std::get<1>(index);
        int pos = std::get<2>(index);
        if(result.contains(word))
        {
            if(result.value(word).contains(id))
            {
                result[word][id].append(pos);
            }
            else
            {
                QList<int> positions;
                positions.append(pos);
                result[word].insert(id, positions);
            }
        }
        else
        {
            // create an index and store into the inverted list
            Index index;
            QList<int> positions;
            positions.append(pos);
            index.insert(id, positions);
            result.insert(word, index);
        }
    }

    _core->progress("Loading Webpages", WEBPAGES_PER_THREAD);
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
    WEBPAGES_PER_THREAD = float(TOTAL_WEBPAGES - from + 1) / QThread::idealThreadCount();

    this->webpagesCount += TOTAL_WEBPAGES - from + 1;
    this->maxProgress = (TOTAL_WEBPAGES - from + 1) + WEBPAGES_PER_THREAD * (QThread::idealThreadCount() + 1) + 1;

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

    // optional: squeeze all the index hashes in to inverted list
    for(auto iter = this->invertedList.begin(); iter != this->invertedList.end(); ++iter)
        iter.value().squeeze();
    this->invertedList.squeeze();

    this->hasLoaded = true;
    emit this->progress("Loading Webpages", 1);
}


void SearchCore::query(const QString &sentence)
{
    if(!this->hasLoaded)
        qFatal("Core hasn't loaded anything yet.");

    WordSegmenter ws = WordSegmenter(&this->dictionary);
    QStringList keywords = ws.segment(sentence);
    QList<Webpage> webpages;

    QSqlQuery query(this->db);
    for(QString & word : keywords)
    {
        Webpage webpage;

        const QHash<int, QList<int> > & wordHash = this->invertedList.value(word);
        for(auto iter = wordHash.begin(); iter != wordHash.end(); ++iter)
        {
            query.prepare("SELECT title, content, url from `webpages` WHERE id == :id");
            query.bindValue(":id", iter.key());
            if(!query.exec() || !query.next())
                qFatal(query.lastError().text().toLatin1().data());

            webpage.title = query.value(0).toString();
            // TODO: find the best-fit brief
            webpage.brief = query.value(1).toString().mid(0, 30);
            webpage.url = query.value(2).toString();
            query.clear();
        }
        webpages.append(webpage);
    }

    // TODO: evaluate and order the results
    emit this->result(keywords, webpages);
    return;
}


void SearchCore::clear()
{
    this->invertedList.clear();
    this->webpagesCount = 0;
}
