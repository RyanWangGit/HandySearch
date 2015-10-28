/*************************************
 * Copyright(C),2015-2016,Ryan Wang 
 * 
 * File:	HandySearch.cpp
 *
 * Version: V1.0
 * 
 * Brief:	This is the implementations of HandySearch class,
 * which is the main QT class that controls and manages every
 * QT objects and main window.
 *
 * Author:	Ryan
 
 * Date:	Oct. 2015
*************************************/
#include "stdafx.h"
#include "List.h"
#include "Html.h"
#include "Index.h"
#include "HandySearch.h"
#include "WordSegmenter.h"
#include "LoadUI.h"

#define DEBUG

/*--------------------------
* HandySearch::HandySearch
* 	The default constructor of HandySearch,setup UI andconnect signal and
* slots between loadUI and HandySearch class,and start initiating threads.
* Parameter:
* 	QWidget * parent - Parent of MainWindow.
----------------------------*/
HandySearch::HandySearch(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->isResultShown = false;
	this->setMinimumHeight(MINHEIGHT);
	this->setMinimumWidth(MINWIDTH);
	this->setWindowIconText("HandySearch");

	connect(this->ui.resultEdit, &QTextBrowser::anchorClicked, this, &HandySearch::anchorClicked);
	
	//LoadUI initializations
	QThread* loadUIThread = new QThread();
	LoadUI* loadUI = new LoadUI();
	loadUI->moveToThread(loadUIThread);
	connect(loadUIThread, &QThread::started, loadUI, &LoadUI::loadData);
	connect(loadUI, &LoadUI::canceled, this, &HandySearch::loadCanceled);
	connect(loadUI, &LoadUI::loadFinished, loadUI, &QObject::deleteLater);
	connect(loadUI, &LoadUI::loadFinished, loadUIThread, &QThread::quit);
	connect(loadUI, &LoadUI::loadFinished, loadUIThread, &QObject::deleteLater);
	connect(loadUI, &LoadUI::finished, this, &HandySearch::loadFinished);
	loadUIThread->start();
}


/*--------------------------
* HandySearch::segment
* 	Retrive the text in searchEdit and do segmentations,
* and put the result back in searchEdit.
----------------------------*/
void HandySearch::segment()
{
	WordSegmenter ws(this->ui.searchEdit->text(), this->dictionary);
	QString result;
	QStringList wordList = ws.getResult();
	for (QString word : wordList)
		result.append(word + "/");
	this->ui.searchEdit->setText(result);
}


/*--------------------------
* HandySearch::search
* 	Set the UI Layout into result mode,collect the search result
* in the inverted list and sort it,then show the result.
----------------------------*/
void HandySearch::search()
{
	this->clock.start();
	if (this->ui.searchEdit->text() == "")
	{
		this->setDefaultUILayout();
		return;
	}

	//Put all results into one sorted list
	//List<Index*> sortedList;
	List<Index*> titleList;
	List<Index*> contentList;

	QString searchContent = this->ui.searchEdit->text().mid(0,20);
	WordSegmenter ws(searchContent, this->dictionary);
	QStringList wordList = ws.getResult();
	wordList.removeDuplicates();
	wordList.removeAll(" ");
	
	for (QString word : wordList)
	{
		//Get the indexList from inverted list
		List<Index>* indexList = nullptr;
		List<Index>** pIndexList = HandySearch::index.get(word);
		if (pIndexList == nullptr)
			continue;
		else
			indexList = *pIndexList;
		
		//Traverse all index and put them into sorted list
		for (int i = 0; i < indexList->size(); i++)
		{
			Index* index = &indexList->get(i);
			Html* html = index->getHtml();
			//this->putInSortedList(index, sortedList);
			
			bool isInTitle = false;
			for (QString word : wordList)
			{
				
				if (html->getTitle().contains(word))
				{
					isInTitle = true;
					index->getRefWeight()++;
				}		
			}

			//Collect those have keywords in title
			if (isInTitle)
				this->putInTitleList(index, titleList);
			//Collect those don't
			else
				this->putInContentList(index, contentList);
		}
	}
	this->ui.resultEdit->clear(); 
	this->showResult(titleList.append(contentList), wordList);
}

/*
//Auto sort when putting index into list by weight
void HandySearch::putInSortedList(Index* index, List<Index *>& list)
{
	Html* html = index->getHtml();

}*/


/*--------------------------
* HandySearch::putInTitleList
* 	Auto sort when putting index into list by weight.
* Parameter:
* 	Index * index - An index.
* 	List<Index * > & list - List of indexes which contains title.
----------------------------*/
void HandySearch::putInTitleList(Index* index, List<Index *>& list)
{
	bool hasFound = false;
	for (int i = 0; i < list.size(); i++)
	{
		if (list.get(i)->getRefWeight() >= index->getRefWeight())
			continue;
		else
		{
			hasFound = true;
			list.insertAfter(i - 1, index);
			break;
		}
	}
	if (!hasFound)
		list.append(index);
}


/*--------------------------
* HandySearch::putInContentList
* 	Auto sort when putting index into list by weight.
* Parameter:
* 	Index * newIndex - An index.
* 	List<Index * > & list - List of indexes which key words are in content.
----------------------------*/
void HandySearch::putInContentList(Index* newIndex, List<Index *>& list)
{
	Html *html = newIndex->getHtml();
	bool hasFound = false;
	for (int i = 0; i < list.size(); i++)
	{
		Index* index = list.get(i);
		//If found duplicate index
		if (index->getHtml()->getTitle() == html->getTitle())
		{
			index->getRefWeight() += newIndex->getFrequency();
			//Iterate forwards to find one whose weight
			//is higher than current one
			for (int j = i - 1; j >= 0; j--)
			{
				if (list.get(j)->getRefWeight() > index->getRefWeight())
				{
					list.insertAfter(j, index);
					list.remove(i + 1);
					break;
				}
			}
			hasFound = true;
			break;
		}
	}
		
	if (!hasFound)
	{
		newIndex->getRefWeight() = newIndex->getFrequency();
		list.append(newIndex);
	}
}

/*--------------------------
* HandySearch::anchorClicked
* 	Open the url in native browser.
* Parameter:
* 	const QUrl & url - The url user clicked.
----------------------------*/
void HandySearch::anchorClicked(const QUrl& url)
{
	QDesktopServices::openUrl(QUrl("file:///" + url.toString()));
}


/*--------------------------
* HandySearch::about
* 	Show about dialog.
----------------------------*/
void HandySearch::about()
{
	this->segment();
}

/*--------------------------
* HandySearch::resizeEvent
* 	Handle resize event.
* Parameter:
* 	QResizeEvent * event - The resize event.
----------------------------*/
void HandySearch::resizeEvent(QResizeEvent *event)
{
	if (!isResultShown)
		this->setDefaultUILayout();
	else
		this->setResultUILayout();
}


/*--------------------------
* HandySearch::setDefaultUILayout
* 	Set defaulkt UI layout,which puts logo in the center
* and searchEdit below the logo.
----------------------------*/
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


/*--------------------------
* HandySearch::setResultUILayout
* 	Set result-showing UI layout which puts logo in the left-top
* cornor and searchEdit at its right,with resultEdit put below them.
----------------------------*/
void HandySearch::setResultUILayout()
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


/*--------------------------
* HandySearch::showResult
* 	Organize the result in the resultList and show them in the 
* resultEdit.
* Parameter:
* 	List<Index * > & resultList - List of results.
* 	QStringList & wordList - List of key words
----------------------------*/
void HandySearch::showResult(List<Index*> &resultList, QStringList &wordList)
{
	this->setResultUILayout();
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


/*--------------------------
* HandySearch::loadCanceled
* 	Quit application when initiating load was canceled.
----------------------------*/
void HandySearch::loadCanceled()
{
	QApplication::quit();
}


/*--------------------------
* HandySearch::loadFinished
* 	Show the main window and set the completer.
----------------------------*/
void HandySearch::loadFinished()
{
	this->show();
	//Set Auto completer
	HandySearch::sentences.removeDuplicates();
	this->completer = new QCompleter(HandySearch::sentences, this);
	this->ui.searchEdit->setCompleter(completer);
}

