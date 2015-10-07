#include "stdafx.h"

Load::Load(const QString &htmlFolder, const QString &dictFolder)
{
	this->htmlFolder = htmlFolder;
	this->dictFolder = dictFolder;
}

void Load::loadDictionary()
{
	/* -----Load dictionary----- */
	emit dictLoadStarted();
	QDirIterator dictIter(dictFolder, QDirIterator::Subdirectories);
	QFile file;
	QString path;
	QString temp;
	int index = 0;
	while (dictIter.hasNext())
	{
		path = dictIter.next();
		file.setFileName(path);
		if (!file.open(QIODevice::OpenModeFlag::ReadOnly | QIODevice::OpenModeFlag::Text))
			continue;
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

void Load::loadHtml()
{
	/* -----Load htmls----- */
	emit htmlLoadStarted();
	QDirIterator htmlIter(htmlFolder, QDirIterator::Subdirectories);

	QStringList pathList;
	int i = 0;
	while (htmlIter.hasNext())
	{
		QString path = htmlIter.next();
		if (path.endsWith("."))
			continue;
		pathList.append(path);
		i++;
		if (i == 250)
		{
			QThread *loadHtmlThread = new QThread();
			LoadHtml *loadHtml = new LoadHtml(pathList);
			loadHtml->moveToThread(loadHtmlThread);
			connect(loadHtmlThread, &QThread::started, loadHtml, &LoadHtml::load);
			connect(loadHtml, &LoadHtml::finished, this, &Load::htmlThreadFinished);
			connect(loadHtml, &LoadHtml::finished, loadHtml, &QObject::deleteLater);
			connect(loadHtml, &LoadHtml::finished, loadHtmlThread, &QThread::quit);
			connect(loadHtmlThread, &QThread::finished, loadHtmlThread, &QObject::deleteLater);
			connect(loadHtml, &LoadHtml::processHtml, this, &Load::processHtml, Qt::QueuedConnection);

			loadHtmlThread->start();
			pathList.clear();
			i = 0;
		}
	}
	//The remaining
	if (pathList.size() != 0)
	{
		QThread *loadHtmlThread = new QThread();
		LoadHtml *loadHtml = new LoadHtml(pathList);
		loadHtml->moveToThread(loadHtmlThread);
		connect(loadHtmlThread, &QThread::started, loadHtml, &LoadHtml::load);
		connect(loadHtml, &LoadHtml::finished, this, &Load::htmlThreadFinished);
		connect(loadHtml, &LoadHtml::finished, loadHtml, &QObject::deleteLater);
		connect(loadHtml, &LoadHtml::finished, loadHtmlThread, &QThread::quit);
		connect(loadHtmlThread, &QThread::finished, loadHtmlThread, &QObject::deleteLater);
		connect(loadHtml, &LoadHtml::processHtml, this, &Load::processHtml, Qt::QueuedConnection);

		loadHtmlThread->start();
		i = 0;
	}
	/* ---------- */
}

void Load::processHtml(unsigned int threadID, Html* html, QString path)
{
	html->analyze();
	Html::totalNum++;
	QString sentence = html->getTitle();
	sentence.replace(QRegExp("[\s | _|,|\|.|;|\|:|\|'|\"|¡£|£¬|~|£¡|£¡|@|#|$|%|^|&|*|£¨|£©|(|)|!]*"), "");
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


//Html load threadlet
LoadHtml::LoadHtml(const QStringList &pathList)
{
	threadNum++;
	this->id = threadNum;
	this->pathList = pathList;
}
