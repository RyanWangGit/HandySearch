#include "stdafx.h"
#include "handysearch.h"
#define DEBUG

HandySearch::HandySearch(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->ui.clear->hide();
	this->isResultShown = false;
	this->setMinimumHeight(MINHEIGHT);
	this->setMinimumWidth(MINWIDTH);
	this->setWindowIconText("HandySearch");

	connect(this->ui.resultEdit, &QTextBrowser::anchorClicked, this, &HandySearch::anchorClicked);


}


void HandySearch::test()
{
	//qDebug() << this->ui.textEdit->toPlainText();
	WordSegmenter ws(this->ui.searchEdit->text(), this->dictionary);
	QString result;
	QStringList qsl = ws.getResult();
	for (QString word : qsl)
		result.append(word + "/");
	this->ui.searchEdit->setText(result);
}

void HandySearch::search()
{
	this->clock.start();
	if (this->ui.searchEdit->text() == "")
	{
		this->setDefaultUILayout();
		return;
	}
	List<Index*> resultList;
	QString searchContent = this->ui.searchEdit->text();
	WordSegmenter ws(this->ui.searchEdit->text(), this->dictionary);
	QStringList qsl = ws.getResult();
	qsl.removeDuplicates();
	qsl.removeAll(" ");
	
	for (QString word : qsl)
	{
		List<Index>* indexList = nullptr;
		List<Index>** pIndexList = HandySearch::index.get(word);
		if (pIndexList == nullptr)
			continue;
		else
			indexList = *pIndexList;

		for (int i = 0; i < indexList->size(); i++)
		{
			Index* index = &indexList->get(i);
			Html *html = index->getHtml();
			bool hasFound = false;
			for (int j = 0; j < resultList.size(); j++)
				if (resultList.get(j)->getHtml()->getTitle() == html->getTitle())
					hasFound = true;
			if (!hasFound)
				resultList.append(index);
		}
	}
	this->ui.resultEdit->clear();
	this->showResult(resultList, qsl);
}

void HandySearch::textChanged()
{
	if (this->ui.searchEdit->text() == "")
		this->ui.clear->hide();
	else
		this->ui.clear->show();
}

void HandySearch::anchorClicked(const QUrl& url)
{
	ShellExecuteA(NULL, "open", url.toString().toStdString().c_str(), "", "", SW_SHOW);
}


void HandySearch::about()
{
	
}

void HandySearch::resizeEvent(QResizeEvent *event)
{
	if (!isResultShown)
		this->setDefaultUILayout();
	else
		this->setShownUILayout();
}

void HandySearch::mouseMoveEvent(QMouseEvent *event)
{
	if (this->ui.clear->rect().contains(event->pos())
		|| this->ui.search->rect().contains(event->pos())
		)
		this->setCursor(Qt::PointingHandCursor);
	else
		this->setCursor(Qt::ArrowCursor);

}

void HandySearch::setDefaultUILayout()
{
	this->ui.about->show();
	this->ui.segment->hide();
	this->ui.resultEdit->hide();
	this->isResultShown = false;
	this->ui.copyright->setText("Designed and Powered By:\nRyan Wang @ HUST");
	this->ui.copyright->setFont(QFont(QStringLiteral("Segoe UI Light"), 14));
	this->ui.logo->setGeometry(
		this->width() / 2 - 356 / 2,
		this->height() / 2 - 97 / 2 - 100,
		356,
		97
		);
	this->ui.logo->setScaledContents(true);
	this->ui.searchEdit->move(QPoint(
		this->ui.logo->x() - 80,
		this->ui.logo->y() + this->ui.logo->height() + 20
		));
	this->ui.search->move(QPoint(
		this->ui.searchEdit->x() + this->ui.searchEdit->width(),
		this->ui.searchEdit->y())
		);
	this->ui.clear->move(QPoint(
		this->ui.search->x() - this->ui.clear->width(),
		this->ui.search->y())
		);
	this->ui.copyright->setGeometry(
		this->width() / 2 - this->ui.copyright->width() / 2,
		this->height() / 2 + 100,
		251,
		81
		);
	this->ui.about->move(QPoint(
		this->width() / 2 - this->ui.about->width() / 2,
		this->height() - this->ui.about->height()
		));
}

void HandySearch::setShownUILayout()
{
	this->isResultShown = true;
	this->ui.about->hide();
	this->ui.segment->show();
	this->ui.resultEdit->show();
	this->ui.copyright->setText("Designed and Powered by : Ryan Wang @ HUST");
	this->ui.copyright->setFont(QFont(QStringLiteral("Segoe UI Light"), 8));
	this->ui.logo->setGeometry(5, 5, 150, 41);
	this->ui.logo->setScaledContents(true);
	this->ui.searchEdit->move(QPoint(
		this->ui.logo->x() + this->ui.logo->width() + 20,
		this->ui.logo->y()
		));

	this->ui.searchEdit->setFixedWidth(471 + this->width() - MINWIDTH);

	this->ui.search->move(QPoint(
		this->ui.searchEdit->x() + this->ui.searchEdit->width(),
		this->ui.searchEdit->y())
		);
	this->ui.clear->move(QPoint(
		this->ui.search->x() - this->ui.clear->width(),
		this->ui.search->y())
		);
	this->ui.segment->setGeometry(
		0,
		this->ui.logo->y() + this->ui.logo->height() + 10,
		this->width(),
		20
		);
	this->ui.resultEdit->setGeometry(
		this->ui.segment->x() + 15,
		this->ui.segment->y() + 30,
		this->width() - 30,
		this->height() - this->ui.segment->y() - 45
		);
	this->ui.copyright->setGeometry(
		this->width() - 221 - 15,
		this->height() - 16,
		221,
		16
		);
}

void HandySearch::showResult(List<Index*> &resultList, QStringList &wordList)
{
	this->setShownUILayout();
	QString resultContent(this->ui.resultEdit->toHtml());
	for (int i = 0; i < resultList.size(); i++)
	{
		Index* index = resultList.get(i);
		QString brief = index->getHtml()->getText().mid(index->getPosition().get(0) - HandySearch::dictionary.getMaxLength(), 200);
		brief.replace(QRegExp("[\n || \t ]"), "");
		resultContent.append("<a href=\"" + index->getHtml()->getFilePath() + "\"><font size = \"5\">" + index->getHtml()->getTitle() + "</font></a>");
		resultContent.append("<br>&emsp;......" + brief + "......" + "<br><br>");
	}

	for (QString word : wordList)
		resultContent.replace(word, "<font color=\"#cc0000\">" + word + "</font>");

	this->ui.resultEdit->setHtml(resultContent);
	this->ui.segment->setText("   HandySearch has provided " + QString::number(resultList.size()) + " result(s) for you in " + QString::number((double)this->clock.elapsed() / 1000) + " second(s)");
	this->clock.restart();
}

void HandySearch::loadFinished()
{
	this->show();
	//Set Auto completer
	HandySearch::sentences.removeDuplicates();
	this->completer = new QCompleter(HandySearch::sentences, this);
	this->ui.searchEdit->setCompleter(completer);
}

//This is a test method
void HandySearch::onCloseButtonClick()
{
	try
	{
		QApplication::exit();
	}	
	catch (...)
	{
		qDebug() << "Shutdown Error";
	}
		
	
}
