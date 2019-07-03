#include <QDesktopServices>
#include <QCompleter>
#include "handy_search.h"
#include "word_segmenter.h"
#include "load_ui.h"
#include "search_core.h"


HandySearch::HandySearch(QWidget *parent)
  : QMainWindow(parent), MINWIDTH(850), MINHEIGHT(500)
{
  ui.setupUi(this);
  /* Initialize variables */
  isResultShown = false;

  setMinimumHeight(MINHEIGHT);
  setMinimumWidth(MINWIDTH);
  setWindowIconText("HandySearch");

  this->core.moveToThread(&this->coreThread);
  connect(this, &HandySearch::progress, &this->loadUI, &LoadUI::progress);
  connect(this, &HandySearch::startLoading, &this->core, &SearchCore::load);
  connect(this->ui.resultEdit, &QTextBrowser::anchorClicked, [](const QUrl &url) { QDesktopServices::openUrl(url); });

  connect(&this->loadUI, &LoadUI::start, [this](const QString &databasePath) {
    // use bundled dictionary file
    this->core.setPath(":/assets/dictionary.txt", databasePath);
    emit this->startLoading();
  } );
  connect(&this->core, &SearchCore::progress, [this](const QString &hint, uint progress) {
    static uint totalProgress = 0;
    totalProgress += progress;
    emit this->progress(hint, totalProgress / float(this->core.getMaxProgress()));

    if(totalProgress == this->core.getMaxProgress())
      emit this->loadFinished();

  });
  connect(this, &HandySearch::loadFinished, this, &HandySearch::handleFinished);

  connect(this->ui.searchEdit, &QLineEdit::returnPressed, [this]() { emit this->query(this->ui.searchEdit->text()); });
  connect(this->ui.search, &QPushButton::clicked, [this]() { emit this->query(this->ui.searchEdit->text()); });
  connect(this, &HandySearch::query, &this->core, &SearchCore::query);
  connect(&this->core, &SearchCore::result, this, &HandySearch::searchResult);
  this->coreThread.start();
}


void HandySearch::handleFinished()
{
  this->show();
  this->loadUI.hide();

  /* Set Auto completer */
  this->completer = new QCompleter(this->core.getTitleList(), this);
  this->ui.searchEdit->setCompleter(completer);
}


bool HandySearch::load()
{
  loadUI.show();
  loadUI.loadData();
  return true;
}


void HandySearch::searchResult(const QStringList &keywords, const QList<Webpage> &webpages)
{
  ui.resultEdit->clear();
  showResult(keywords, webpages);
}


void HandySearch::resizeEvent(QResizeEvent *event)
{
  if (!isResultShown)
    setDefaultUILayout();
  else
    setResultUILayout();
}


void HandySearch::setDefaultUILayout()
{
  ui.about->show();
  ui.segment->hide();
  ui.resultEdit->hide();
  isResultShown = false;
  ui.copyright->setText("Designed and Powered By:\nRyan Wang @ HUST");
  ui.copyright->setFont(QFont(QStringLiteral("Segoe UI Light"), 14));
  ui.logo->setGeometry(
    width() / 2 - 356 / 2,
    height() / 2 - 97 / 2 - 100,
    356,
    97
    );
  ui.logo->setScaledContents(true);
  ui.searchEdit->move(QPoint(
    ui.logo->x() - 80,
    ui.logo->y() + ui.logo->height() + 20
    ));
  ui.search->move(QPoint(
    ui.searchEdit->x() + ui.searchEdit->width(),
    ui.searchEdit->y())
    );
  ui.copyright->setGeometry(
    width() / 2 - ui.copyright->width() / 2,
    height() / 2 + 100,
    251,
    81
    );
  ui.about->move(QPoint(
    width() / 2 - ui.about->width() / 2,
    height() - ui.about->height()
    ));
}


void HandySearch::setResultUILayout()
{
  isResultShown = true;
  ui.about->hide();
  ui.segment->show();
  ui.resultEdit->show();
  ui.copyright->setText("Designed and Powered by : Ryan Wang @ HUST");
  ui.copyright->setFont(QFont(QStringLiteral("Segoe UI Light"), 8));
  ui.logo->setGeometry(5, 5, 150, 41);
  ui.logo->setScaledContents(true);
  ui.searchEdit->move(QPoint(
    ui.logo->x() + ui.logo->width() + 20,
    ui.logo->y()
    ));

  ui.searchEdit->setFixedWidth(471 + width() - MINWIDTH);

  ui.search->move(QPoint(
    ui.searchEdit->x() + ui.searchEdit->width(),
    ui.searchEdit->y())
    );
  ui.segment->setGeometry(
    0,
    ui.logo->y() + ui.logo->height() + 10,
    width(),
    20
    );
  ui.resultEdit->setGeometry(
    ui.segment->x() + 15,
    ui.segment->y() + 30,
    width() - 30,
    height() - ui.segment->y() - 45
    );
  ui.copyright->setGeometry(
    width() - 221 - 15,
    height() - 16,
    221,
    16
    );
}


void HandySearch::showResult(const QStringList &keywords, const QList<Webpage> &webpages)
{
  setResultUILayout();
  QString resultContent(ui.resultEdit->toHtml());
  
  for (const Webpage &webpage : webpages)
  {
    resultContent.append("<a href=\"" + webpage.url + "\"><font size = \"5\">" + webpage.title + "</font></a>");
    resultContent.append("<br>&emsp;......" + webpage.brief + "......" + "<br><br>");
  }

  for (const QString &word : keywords)
    resultContent.replace(word, "<font color=\"#cc0000\">" + word + "</font>");

  ui.resultEdit->setHtml(resultContent);
  ui.segment->setText("   HandySearch has provided " + QString::number(webpages.size()) + " result(s) for you in " + QString::number((double)clock.elapsed() / 1000) + " second(s)");

  clock.restart();
}
