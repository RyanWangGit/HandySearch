#include <memory>
#include <tuple>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDir>
#include <QtConcurrent>
#include "qjieba.hpp"
#include "searchcore.h"


// <id, positions>
typedef QHash<int, QList<int> > Index;
// < word, index >
typedef QHash<QString, Index> InvertedList;

SearchCore::SearchCore(const QString &database)
{
  this->setPath(database);
  this->hasLoaded = false;
  this->maxProgress = 0;
  this->webpagesCount = 0;
}


SearchCore::SearchCore()
{
  this->hasLoaded = false;
  this->maxProgress = 0;
  this->webpagesCount = 0;
}

SearchCore::~SearchCore()
{
  this->db.close();
}


void SearchCore::setPath(const QString &database)
{
  this->databasePath = database;
}


const QString &SearchCore::getDatabasePath() const
{
  return this->databasePath;
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

QString SearchCore::copyEmbedded(const QString &path)
{
  QFile embedded(path);
  if(!embedded.exists())
    qFatal("Embedded file doesn\'t exist");
  QString location = QStandardPaths::writableLocation(
        QStandardPaths::TempLocation);

  if(location.isEmpty())
    qFatal("Could not obtain writable location for test database file");

  QString copyLocation = QDir::cleanPath(location + QDir::separator() +
                                         path.section('/', -1, -1));

  embedded.copy(copyLocation);
  QFile::setPermissions(copyLocation, QFile::ReadOwner | QFile::WriteOwner);
  return copyLocation;
}

/* TODO: need more elegant solution */
// used by mapper and reducer since they have to be static functions
static SearchCore *_core = nullptr;

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
  query.prepare("SELECT id, title, content from `webpages` "
                "WHERE ROWID >= :start AND ROWID <= :end");
  query.bindValue(":start", task.first);
  query.bindValue(":end", task.second);
  if(!query.exec())
    qFatal("Database query failure: \"%s\"",
           query.lastError().text().toLatin1().constData());

  QList<std::tuple<QString, int, int> > indexList;

  const int PROGRESS_FREQUENCY = 10;
  uint count = 0, reported = 0;
  while(query.next())
  {
    int id = query.value(0).toInt();

    int pos = 0;

    // use negative position to indicate that word is in title
    for(const QString &word :
        _core->wordSegmenter->cut(query.value(1).toString()))
    {

      pos -= word.size();
      indexList.append(std::make_tuple(word, id, pos));
    }

    pos = 0;
    for(const QString &word :
        _core->wordSegmenter->cut(query.value(2).toString()))
    {
      pos += word.size();
      indexList.append(std::make_tuple(word, id, pos));
    }

    if(count % PROGRESS_FREQUENCY == 0 && count != 0)
    {
      _core->progress(PROGRESS_FREQUENCY);
      reported += PROGRESS_FREQUENCY;
    }

    count++;
  }

  // report the remaining progress
  if(count - reported != 0)
    _core->progress(count - reported);

  db.close();

  return indexList;
}


// get <word, id, pos> from mapper and store into the inverted list
void reducer(InvertedList &result, const QList<std::tuple<QString, int, int> > &other)
{
  if(result.isEmpty())
    result.reserve(static_cast<int>(_core->getWebpagesCount()));

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
      Index newIndex;
      QList<int> positions;
      positions.append(pos);
      newIndex.insert(id, positions);
      result.insert(word, newIndex);
    }
  }
}


void SearchCore::load(uint from)
{
  // setup word segmenter
  QString jieba = this->copyEmbedded(":/libs/cppjieba/dict/jieba.dict.utf8");
  QString hmmModel = this->copyEmbedded(":/libs/cppjieba/dict/hmm_model.utf8");
  QString user = this->copyEmbedded(":/libs/cppjieba/dict/user.dict.utf8");
  QString idf = this->copyEmbedded(":/libs/cppjieba/dict/idf.utf8");
  QString stopWords = this->copyEmbedded(":/libs/cppjieba/dict/stop_words.utf8");

  this->wordSegmenter = std::make_unique<QJieba>(jieba, hmmModel, user, idf, stopWords);

  // open database
  this->db = QSqlDatabase::addDatabase("QSQLITE", QUuid::createUuid().toString());
  this->db.setDatabaseName(this->databasePath);

  if(!this->db.open())
  {
    qFatal("Database cannot be opened with error \"%s\".",
           this->db.lastError().text().toLatin1().constData());
  }

  QSqlQuery query("SELECT COUNT(id) FROM `webpages`", this->db);
  if(!query.next())
    qFatal("Database execution failed with error \"%s\".",
           query.lastError().text().toLatin1().constData());

  // assign the workload
  const uint TOTAL_WEBPAGES = query.value(0).toUInt();
  uint WEBPAGES_PER_THREAD = static_cast<uint>(
        float(TOTAL_WEBPAGES - from + 1) / QThread::idealThreadCount());

  this->webpagesCount += TOTAL_WEBPAGES - from + 1;
  this->maxProgress = (TOTAL_WEBPAGES - from + 1) + 1;

  QList<QPair<int, int> > tasks;
  for(uint i = from; i < TOTAL_WEBPAGES; i += WEBPAGES_PER_THREAD)
  {
    if(i + WEBPAGES_PER_THREAD >= TOTAL_WEBPAGES)
      tasks.append(QPair<uint, uint>(i, TOTAL_WEBPAGES));
    else
      tasks.append(QPair<uint, uint>(i, i + WEBPAGES_PER_THREAD - 1));
  }

  // set static global variable to be used in mapper and reducer
  _core = &(*this);

  // mapreduce to process all the webpages
  this->invertedList = QtConcurrent::blockingMappedReduced<InvertedList>(
        tasks, mapper, reducer, QtConcurrent::UnorderedReduce);

  // optional: squeeze all the index hashes in to inverted list
  for(auto iter = this->invertedList.begin(); iter != this->invertedList.end(); ++iter)
    iter.value().squeeze();
  this->invertedList.squeeze();

  this->hasLoaded = true;
  emit this->progress(1);
}


void SearchCore::query(const QString &sentence)
{
  if(!this->hasLoaded)
    qFatal("Core hasn't loaded anything yet.");

  QStringList keywords = _core->wordSegmenter->cut(sentence);
  QList<Webpage> webpages;

  QSqlQuery query(this->db);
  for(QString & word : keywords)
  {
    const QHash<int, QList<int> > & wordHash = this->invertedList.value(word);
    for(auto iter = wordHash.begin(); iter != wordHash.end(); ++iter)
    {
      query.prepare("SELECT title, content, url from `webpages` WHERE id == :id");
      query.bindValue(":id", iter.key());
      if(!query.exec() || !query.next())
        qFatal("Database query failure: \"%s\"",
               query.lastError().text().toLatin1().constData());
      Webpage webpage;
      webpage.title = query.value(0).toString();
      // TODO: find the best-fit brief
      webpage.brief = query.value(1).toString().mid(0, 300);
      webpage.url = query.value(2).toString();
      query.clear();
      webpages.append(webpage);
    }
  }

  // TODO: evaluate and order the results
  emit this->result(keywords, webpages);
}


void SearchCore::clear()
{
  this->invertedList.clear();
  this->webpagesCount = 0;
}
