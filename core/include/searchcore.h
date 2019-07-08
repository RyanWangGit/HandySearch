#pragma once

#include <QString>
#include <QStringList>
#include <QList>
#include <QObject>
#include <QHash>
#include <QSqlDatabase>


typedef struct _webpage { QString title, brief, url; } Webpage;

Q_DECLARE_METATYPE(Webpage)

// forward declaration
class QJieba;

class SearchCore : public QObject
{
  Q_OBJECT
private:
  bool hasLoaded;
  QString databasePath;
  QHash<QString, QHash<int, QList<int> > > invertedList;
  QSqlDatabase db;
  unsigned int maxProgress;
  unsigned int webpagesCount;
  QString copyEmbedded(const QString &path);
public:
  SearchCore(const QString &database);
  SearchCore();
  ~SearchCore();
  std::unique_ptr<QJieba> wordSegmenter;
  void setPath(const QString &database);
  const QString &getDatabasePath() const;
  unsigned int getMaxProgress() const;
  unsigned int getWebpagesCount() const;
  QStringList getTitleList() const;
  void load(uint from = 1);
  void query(const QString &sentence);
  void clear();
signals:
  void progress(uint progress);
  void result(const QStringList &keywords, const QList<Webpage> &webpages);
};
