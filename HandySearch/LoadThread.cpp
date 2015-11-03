/*************************************
 * Copyright(C),2015-2016,Ryan Wang 
 * 
 * File:	LoadThread.cpp
 *
 * Version: V1.0
 * 
 * Brief:	These are the implementations of multiple working threads
 * which accelerates the initial loading procedure.
 *
 * Author:	Ryan
 
 * Date:	Oct. 2015
*************************************/
#include "stdafx.h"
#include "Html.h"
#include "HandySearch.h"
#include "LoadThread.h"
#include "LoadUI.h"


/*--------------------------
* LoadHtml::LoadHtml
* 	Html load threadlet constructor.
* Parameter:
* 	const QStringList & pathList - Path of files.
----------------------------*/
HtmlLoadSubTask::HtmlLoadSubTask(const QStringList& pathList)
{
	this->pathList = pathList;
}


/*--------------------------
* HtmlLoadSubTask::run
* 	Load several html files,this is a sub-task,managed by HtmlLoadTask.
----------------------------*/
void HtmlLoadSubTask::run()
{
#ifdef DEBUG
	qDebug() << "[Html Loading Thread #" << QObject::thread()->currentThreadId() << "]" << "Thread Received " << pathList.size() << "Files";
#endif 
	for (QString path : pathList)
	{
#ifndef SKIPLOAD 
		/* Ignore those aren't html files */
		if (!(path.endsWith(".html") || path.endsWith(".htm")))
			continue;

		Html *pHtml = new Html(path);
		QString sentence = pHtml->getTitle();
		/* Remove all pucntuations , both English and Chinese ones */
		sentence.replace(QRegExp("\\pP|\\pS"), "");
		sentence.chop(10);
		HandySearch::sentences.append(sentence);
#ifdef DEBUG
		qDebug() << "[Html Loading Thread # " << QObject::thread()->currentThreadId() << "]" << "Compelete #" << Html::getTotalHtmlCount() << path << pHtml->getTitle();
#endif
		emit htmlLoaded();
#endif
	}
}


/*--------------------------
* DictLoadTask::DictLoadTask
* 	This is the common-used constructor.
* Parameter:
* 	const QDir & dictFolder - Dictionary folder.
----------------------------*/
DictLoadTask::DictLoadTask(const QDir& dictFolder)
{
	this->dictFolder = dictFolder;
}


/*--------------------------
* DictLoadTask::load
* 	Start loading dictionary.
----------------------------*/
void DictLoadTask::load()
{
	emit dictLoadStarted();
	/* List all files */
	QStringList pathList = dictFolder.entryList(QDir::NoDotAndDotDot | QDir::Files);
	for (QString& path : pathList)
		path.prepend(dictFolder.absolutePath() + "/");

	/* Traverse dictionary folder */
	for (QString path : pathList)
	{
		/* Ignore those aren't text files*/
		if (!path.endsWith(".txt"))
			continue;

		/* Open dictionary file */
		QFile file(path);
		if (!file.open(QIODevice::OpenModeFlag::ReadOnly | QIODevice::OpenModeFlag::Text))
			continue;

		/* Read dictionary file */
		for (int i = 0; !file.atEnd(); i++)
		{
			QString entry = file.readLine();
			/* Cut out the last '\n' character */
			entry.chop(1);
#ifndef SKIPLOAD
			HandySearch::dictionary.addItem(entry);
#endif
			if (i % 1000 == 0)
				emit dictLoaded(1000);
		}
		file.close();
	}
	emit dictLoadFinished();
}


/*--------------------------
* HtmlLoadTask::HtmlLoadTask
* 	This is the common-used constructor
* Parameter:
* 	const QDir & htmlFolder - Html Library Folder.
* 	QObject * parent - LoadUI pointer.
----------------------------*/
HtmlLoadTask::HtmlLoadTask(const QDir& htmlFolder, QObject * parent)
{
	/* We need the parent pointer to bind the signal/slot to LoadUI */
	this->parent = parent;
	this->htmlFolder = htmlFolder;
}


/*--------------------------
* HtmlLoadTask::load
* 	Start loading htmls and put all sub-tasks into global thread pool.
----------------------------*/
void HtmlLoadTask::load()
{
	emit htmlLoadStarted();
	/* List all files */
	QStringList pathList = htmlFolder.entryList(QDir::NoDotAndDotDot | QDir::Files);
	for (QString& path : pathList)
		path.prepend(htmlFolder.absolutePath() + "/");

	QThreadPool* threadPool = QThreadPool::globalInstance();

	/* Traverse html folder */
	for (int i = 0; i < pathList.size(); i += 100)
	{
		/* Create multiple sub-threads to load htmls ( every 100 files ) */
		HtmlLoadSubTask* loadHtml = new HtmlLoadSubTask(pathList.mid(i,100));
		connect(loadHtml, &HtmlLoadSubTask::htmlLoaded, static_cast<LoadUI *>(parent), &LoadUI::htmlLoaded);
		threadPool->start(loadHtml);
	}
	/* Wait for all sub-threads finish */
	threadPool->waitForDone();

	//TODO: 
	//Do have some local hashmaps 
	//and merge all the hashmaps here

	emit htmlLoadFinished();
}
