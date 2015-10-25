#pragma once
#include "Html.h"
//#define SKIPLOAD
//#define DEBUG


/**
 * Class:	LoadHtml
 *
 * Brief:	Html load sub-thread class.
 *
 * Date:	Oct. 2015
 */
class LoadHtml : public QObject
{
	Q_OBJECT
private:
	int id;
	QStringList pathList;
public:
	static unsigned int threadNum;
	LoadHtml(const QStringList &pathList);

signals:
	void processHtml(unsigned int, Html*, QString);
	void finished();

public slots:
	void load()
	{
#ifdef DEBUG
		qDebug() << "[Html Loading Thread #" << this->id << "]" << "Thread Received " << pathList.size() << "Files";
#endif 
		for (int i = 0; i < pathList.size(); i++)
		{
#ifndef SKIPLOAD 
			Html *pHtml = new Html(pathList[i]);
			emit processHtml(this->id, pHtml, pathList.at(i));
#endif
		}		
		emit finished();
	}
};


/**
 * Class:	Load
 *
 * Brief:	Initial Load thread,manages the dictionary loading
 * and html loading threadlets.
 *
 * Date:	Oct. 2015
 */
class Load : public QObject
{
	Q_OBJECT
private:
	QDir htmlFolder;
	QDir dictFolder;
public:
	Load(const QDir &htmlFolder, const QDir &dictFolder);

signals:
	void loadStarted();
	void loadFinished();
	//Html Thread Signals
	void htmlLoaded(unsigned int, QString);
	void htmlLoadStarted();
	void htmlLoadFinished();
	//Dictionary Thread Signals
	void dictLoaded(int num);
	void dictLoadStarted();
	void dictLoadFinished();

private slots:
	//Load tasks
	void loadDictionary();
	void loadHtml();

public slots:
	//Initial load thread
	void run()
	{
		connect(this, &Load::loadStarted, this, &Load::loadDictionary);
		connect(this, &Load::dictLoadFinished, this, &Load::loadHtml);
		connect(this, &Load::htmlLoadFinished, this, &Load::loadFinished);

		emit loadStarted();
	}
	void processHtml(unsigned int threadID,Html* html,QString path);
	void htmlThreadFinished();
};


