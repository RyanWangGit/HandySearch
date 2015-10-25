#include "stdafx.h"
#include "Html.h"
#include "HandySearch.h"
#include "LoadThread.h"


/*--------------------------
* Load::Load
* 	Load object constructor,load with html folder and dictonary folder,
* this is the common-used constructor.
* Parameter:
* 	const QDir & htmlFolder - 
* 	const QDir & dictFolder - 
----------------------------*/
Load::Load(const QDir &htmlFolder, const QDir &dictFolder)
{
	this->htmlFolder = htmlFolder;
	this->dictFolder = dictFolder;
}


/*--------------------------
* Load::loadDictionary
* 	Load dictonary from dictionary folder.
----------------------------*/
void Load::loadDictionary()
{
	/* -----Load dictionary----- */
	emit dictLoadStarted();
	QDirIterator dictIter(dictFolder, QDirIterator::Subdirectories);
	QFile file;
	QString path;
	QString temp;

	int index = 0;
	//Traverse dictionary folder
	while (dictIter.hasNext())
	{
		//Open dictionary file
		path = dictIter.next();
		file.setFileName(path);
		if (!file.open(QIODevice::OpenModeFlag::ReadOnly | QIODevice::OpenModeFlag::Text))
			continue;
		//Read dictionary file
		while (!file.atEnd())
		{
			index++;
			temp = file.readLine();
			temp.chop(1);
#ifndef SKIPLOAD
			HandySearch::dictionary.addItem(temp);
#endif
			if (index % 1000 == 0)
				emit dictLoaded(1000);
		}
		file.close();
	}
	emit dictLoadFinished();
	/* ---------- */
}


/*--------------------------
* Load::loadHtml
* 	Load htmls from html folder.
----------------------------*/
void Load::loadHtml()
{
	/* -----Load htmls----- */
	emit htmlLoadStarted();
	QDirIterator htmlIter(htmlFolder, QDirIterator::Subdirectories);

	QStringList pathList;
	int index = 0;
	//Traverse html folder
	while (htmlIter.hasNext())
	{
		QString path = htmlIter.next();
		//Ignore "./" or "../"
		if (path.endsWith("."))
			continue;

		//Create multiple threads to load htmls
		pathList.append(path);
		index++;
		//Create one thread to process html object every 250 files
		if (index == 250)
		{
			QThread *loadHtmlThread = new QThread();
			LoadHtml *loadHtml = new LoadHtml(pathList);
			loadHtml->moveToThread(loadHtmlThread);
			connect(loadHtmlThread, &QThread::started, loadHtml, &LoadHtml::load);
			connect(loadHtml, &LoadHtml::finished, this, &Load::htmlThreadFinished, Qt::QueuedConnection);
			connect(loadHtml, &LoadHtml::finished, loadHtml, &QObject::deleteLater);
			connect(loadHtml, &LoadHtml::finished, loadHtmlThread, &QThread::quit);
			connect(loadHtmlThread, &QThread::finished, loadHtmlThread, &QObject::deleteLater);
			connect(loadHtml, &LoadHtml::processHtml, this, &Load::processHtml, Qt::QueuedConnection);
			
			loadHtmlThread->start();
			pathList.clear();
			index = 0;
		}
	}
	//The remaining files 
	if (pathList.size() != 0)
	{
		QThread *loadHtmlThread = new QThread();
		LoadHtml *loadHtml = new LoadHtml(pathList);
		loadHtml->moveToThread(loadHtmlThread);
		connect(loadHtmlThread, &QThread::started, loadHtml, &LoadHtml::load);
		connect(loadHtml, &LoadHtml::finished, this, &Load::htmlThreadFinished, Qt::QueuedConnection);
		connect(loadHtml, &LoadHtml::finished, loadHtml, &QObject::deleteLater);
		connect(loadHtml, &LoadHtml::finished, loadHtmlThread, &QThread::quit);
		connect(loadHtmlThread, &QThread::finished, loadHtmlThread, &QObject::deleteLater);
		connect(loadHtml, &LoadHtml::processHtml, this, &Load::processHtml, Qt::QueuedConnection);

		loadHtmlThread->start();
		index = 0;
	}
	/* ---------- */
}


/*--------------------------
* Load::processHtml
* 	Process html object.
* Parameter:
* 	unsigned int threadID - The thread ID.
* 	Html * html - Html object pointer.
* 	QString path - Path of html file processed.
----------------------------*/
void Load::processHtml(unsigned int threadID, Html* html, QString path)
{
	html->analyze();
	Html::totalNum++;
	QString sentence = html->getTitle();
	//Remove all pucntuations , both English and Chinese ones
	sentence.replace(QRegExp("[\\s|_|,|\\|.|;|\||:|\|'|\"|¡£|£¿|£¬|~|£¡|£¡|+|@|#|$|%|^|&|*|£¨|£©|(|)|!|-|¡ª|]"), "");
	sentence.chop(10);
	HandySearch::sentences.append(sentence);
#ifdef DEBUG
	qDebug() << "[Html Loading Thread # " << threadID << "]" << "Compelete #" << Html::totalNum << path << html->getTitle();
#endif
	//Transmit the signal to UI thread
	emit htmlLoaded(threadID, path);
}


void Load::htmlThreadFinished()
{
	LoadHtml::threadNum--;
#ifdef DEBUG
	qDebug() << "[Html Loading Thread]" << LoadHtml::threadNum << "Thread(s) Remaining...";
#endif
	if (LoadHtml::threadNum == 0)
		emit this->htmlLoadFinished();
}


/*--------------------------
* LoadHtml::LoadHtml
* 	Html load threadlet constructor.
* Parameter:
* 	const QStringList & pathList - Path of files.
----------------------------*/
LoadHtml::LoadHtml(const QStringList &pathList)
{
	threadNum++;
	this->id = threadNum;
	this->pathList = pathList;
}
