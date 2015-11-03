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
	isResultShown = false;
	setMinimumHeight(MINHEIGHT);
	setMinimumWidth(MINWIDTH);
	setWindowIconText("HandySearch");

	connect(ui.resultEdit, &QTextBrowser::anchorClicked, this, &HandySearch::anchorClicked);
	
	//LoadUI dialog show up
	LoadUI* loadUI = new LoadUI();
	connect(loadUI, &LoadUI::canceled, this, &HandySearch::loadCanceled);
	connect(loadUI, &LoadUI::finished, this, &HandySearch::loadFinished);
	connect(loadUI, &LoadUI::finished, loadUI, &QObject::deleteLater);
	loadUI->loadData();
}


/*--------------------------
* HandySearch::segment
* 	Retrive the text in searchEdit and do segmentations,
* and put the result back in searchEdit.
----------------------------*/
void HandySearch::segment()
{
	WordSegmenter ws(ui.searchEdit->text(), dictionary);
	QStringList wordList = ws.getResult();
	ui.searchEdit->setText(wordList.join("\\"));
}


/*--------------------------
* HandySearch::search
* 	Set the UI Layout into result mode,collect the search result
* in the inverted list and sort it,then show the result.
----------------------------*/
void HandySearch::search()
{
	clock.start();
	if (ui.searchEdit->text() == "")
	{
		setDefaultUILayout();
		return;
	}

	//Put all results into one sorted list
	//List<Index*> sortedList;
	List<Index*> titleList;
	List<Index*> contentList;

	QString searchContent = ui.searchEdit->text().mid(0,20);
	WordSegmenter ws(searchContent, dictionary);
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
			//putInSortedList(index, sortedList);
			
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
				putInTitleList(index, titleList);
			//Collect those don't
			else
				putInContentList(index, contentList);
		}
	}
	ui.resultEdit->clear(); 
	showResult(titleList.append(contentList), wordList);
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
	segment();
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
		setDefaultUILayout();
	else
		setResultUILayout();
}


/*--------------------------
* HandySearch::setDefaultUILayout
* 	Set defaulkt UI layout,which puts logo in the center
* and searchEdit below the logo.
----------------------------*/
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


/*--------------------------
* HandySearch::setResultUILayout
* 	Set result-showing UI layout which puts logo in the left-top
* cornor and searchEdit at its right,with resultEdit put below them.
----------------------------*/
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
	setResultUILayout();
	QString resultContent(ui.resultEdit->toHtml());
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

	ui.resultEdit->setHtml(resultContent);
	ui.segment->setText("   HandySearch has provided " + QString::number(resultList.size()) + " result(s) for you in " + QString::number((double)clock.elapsed() / 1000) + " second(s)");
	clock.restart();
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
	show();
	//Set Auto completer
	HandySearch::sentences.removeDuplicates();
	completer = new QCompleter(HandySearch::sentences, this);
	ui.searchEdit->setCompleter(completer);
}

