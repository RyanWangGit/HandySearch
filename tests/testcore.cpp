#include <QTest>
#include <QDebug>
#include <QTimer>
#include <QSignalSpy>
#include <QStandardPaths>
#include "search_core.h"

class TestCore: public QObject
{
  Q_OBJECT
private:
  SearchCore *core;
  QString dbPath;
private slots:
  void initTestCase()
  {
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
    this->core->setPath(":/assets/dictionary.txt", this->dbPath);
  }

  void testLoad()
  {
    QSignalSpy spy(this->core, &SearchCore::progress);
    this->core->load();
    QVERIFY2(spy.count() > 0, "Must at least emit one progress signal");
    QVERIFY2(this->core->getMaxProgress() > 0, "Max progress of SearchCore must be positive");
    unsigned int totalProgress = 0;
    for(const QList<QVariant> &signal: spy)
      totalProgress += qvariant_cast<unsigned int>(signal.at(1));
    QVERIFY2(totalProgress == this->core->getMaxProgress(),
             QString("Total progress (%1) should be "
                     "the same as max progress (%2)")
             .arg(totalProgress)
             .arg(this->core->getMaxProgress())
             .toLatin1().constData());
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
