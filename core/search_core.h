#pragma once

#include <QString>
#include <QStringList>
#include <QList>
#include <QObject>
#include <QHash>
#include <QSqlDatabase>
#include "dictionary.h"


typedef struct _webpage { QString title, brief, url; } Webpage;


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
  SearchCore(const QString &dictionary, const QString &database);
  SearchCore();
  ~SearchCore();
  void setPath(const QString &dictionary, const QString &database);
  const QString &getDatabasePath() const;
  const Dictionary &getDictionary() const;
  unsigned int getMaxProgress() const;
  unsigned int getWebpagesCount() const;
  QStringList getTitleList() const;
  void load(int from = 1);
  void query(const QString &sentence);
  void clear();
signals:
  void progress(const QString &hint, int progress);
  void result(const QStringList &keywords, const QList<Webpage> &webpages);
};
