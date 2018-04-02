#include "stable.h"
#include "webpage.h"
#include "index.h"
#include "handy_search.h"
#include "word_segmenter.h"
#include "load_ui.h"
#include "inverted_list.h"


HandySearch::HandySearch(QWidget *parent)
    : QMainWindow(parent), MINHEIGHT(500), MINWIDTH(850)
{
    ui.setupUi(this);
    /* Initialize variables */
    isResultShown = false;

    setMinimumHeight(MINHEIGHT);
    setMinimumWidth(MINWIDTH);
    setWindowIconText("HandySearch");

    connect(ui.resultEdit, &QTextBrowser::anchorClicked, this, &HandySearch::anchorClicked);

    /* Turn on separate thread to store Dictionary and inverted list 
        in order to work independently */
    connect(&invertedList, &InvertedList::queryResult, this, &HandySearch::searchResult);
    dictionary.moveToThread(&dictThread);
    invertedList.moveToThread(&listThread);
    dictThread.start();
    listThread.start();
}


Dictionary* HandySearch::getDictionary()
{
    return &dictionary;
}

InvertedList* HandySearch::getInvertedList()
{
    return &invertedList;
}


bool HandySearch::load()
{
    /* Connect LoadUI signals/slots */
    connect(&loadUI, &LoadUI::canceled, this, &HandySearch::loadCanceled);
    connect(&loadUI, &LoadUI::finished, this, &HandySearch::loadFinished);
    return loadUI.loadData();
}

void HandySearch::segment()
{
    WordSegmenter ws(&dictionary);
    QStringList wordList = ws.segment(ui.searchEdit->text());
    ui.searchEdit->setText(wordList.join("\\"));
}


void HandySearch::search()
{
    clock.start();
    if (ui.searchEdit->text() == "")
    {
        setDefaultUILayout();
        return;
    }

    QString searchContent = ui.searchEdit->text().mid(0, 20);
    WordSegmenter ws(&dictionary);
    QStringList wordList = ws.segment(searchContent);
    wordList.removeDuplicates();
    wordList.removeAll(" ");
    invertedList.query(wordList);

    /* TODO: Start searching animations */
}


void HandySearch::searchResult(const QList<Webpage*> &resultList, const QStringList &keyWordList)
{
    ui.resultEdit->clear();
    showResult(resultList, keyWordList);
}


void HandySearch::anchorClicked(const QUrl &url)
{
    QDesktopServices::openUrl(QUrl("file:///" + url.toString()));
}


void HandySearch::about()
{
    segment();
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


void HandySearch::showResult(const QList<Webpage*> &resultList, const QStringList &keyWordList)
{
    setResultUILayout();
    QString resultContent(ui.resultEdit->toHtml());
    
    for (Webpage* html : resultList)
    {
        resultContent.append("<a href=\"" + html->getFilePath() + "\"><font size = \"5\">" + html->getTitle() + "</font></a>");
        resultContent.append("<br>&emsp;......" + html->getBrief() + "......" + "<br><br>");
    }

    for (QString word : keyWordList)
        resultContent.replace(word, "<font color=\"#cc0000\">" + word + "</font>");

    ui.resultEdit->setHtml(resultContent);
    ui.segment->setText("   HandySearch has provided " + QString::number(resultList.size()) + " result(s) for you in " + QString::number((double)clock.elapsed() / 1000) + " second(s)");
    
    /* Clear all html's weight info in result list */
    for (Webpage* html : resultList)
        html->clearWeight();

    clock.restart();
}


void HandySearch::loadCanceled()
{
    QApplication::quit();
}


void HandySearch::loadFinished()
{
    show();
    /* Set Auto completer */
    completer = new QCompleter(invertedList.getTitleList(), this);
    ui.searchEdit->setCompleter(completer);
}

