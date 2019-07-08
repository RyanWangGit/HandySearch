#include <QTest>
#include <QDebug>
#include <QTimer>
#include <QSignalSpy>
#include <QStandardPaths>
#include "searchcore.h"

class TestCore: public QObject
{
  Q_OBJECT
private:
  SearchCore *core;
  QString dbPath;
private slots:
  void initTestCase()
  {
    qRegisterMetaType<QList<Webpage> >("QList<Webpage>");
    this->core = new SearchCore();
    QFile dbFile(":/tests/test.sqlite");
    if(!dbFile.exists())
      qFatal("Embedded database file doesn\'t exist");
    this->dbPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    if(this->dbPath.isEmpty())
      qFatal("Could not obtain writable location for test database file");
    this->dbPath.append("/test.sqlite");
    dbFile.copy(this->dbPath);
    QFile::setPermissions(this->dbPath, QFile::ReadOwner | QFile::WriteOwner);
    this->core->setPath(this->dbPath);
  }

  void testLoad()
  {
    QSignalSpy spy(this->core, &SearchCore::progress);
    this->core->load();
    QVERIFY2(spy.count() > 0, "Must at least emit one progress signal");
    QVERIFY2(this->core->getMaxProgress() > 0, "Max progress of SearchCore must be positive");
    uint totalProgress = 0;
    for(const QList<QVariant> &signal: spy)
      totalProgress += qvariant_cast<uint>(signal.at(0));
    QVERIFY2(totalProgress == this->core->getMaxProgress(),
             QString("Total progress (%1) should be "
                     "the same as max progress (%2)")
             .arg(totalProgress)
             .arg(this->core->getMaxProgress())
             .toLatin1().constData());
  }

  void testQuery()
  {
    QSignalSpy spy(this->core, &SearchCore::result);
    QString query = "这是一个测试用例Test Case";
    this->core->query(query);
    QVERIFY2(spy.count() == 1, "Should only emit one result for one query");
    QStringList keywords = qvariant_cast<QStringList>(spy.at(0).at(0));
    QList<Webpage> webpages = qvariant_cast<QList<Webpage> >(spy.at(0).at(1));
    QVERIFY2(keywords.length() > 1, "Keyword is not splitted at all");
    for(const QString &word: keywords)
      QVERIFY2(query.contains(word), "Keyword list contains invalid word");
  }

  void cleanupTestCase()
  {
    delete this->core;
    if(!QFile::remove(this->dbPath))
      qWarning() << "Cannot remove generated database file, "
                     "please manually remove it at \"\"" << this->dbPath;
  }
};

QTEST_MAIN(TestCore)
#include "testcore.moc"
