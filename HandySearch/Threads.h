#pragma once

class LoadHtml : public QObject
{
	Q_OBJECT
private:
	int id;
	QStringList pathList;
public:
	static unsigned int threadNum;
	LoadHtml(const QStringList &pathList)
	{
		threadNum++;
		this->id = threadNum; 
		this->pathList = pathList;
	}

signals:
	void processHtml(unsigned int, Html, QString);
	void finished();

public slots:
	void load()
	{
		qDebug() << "[Html Loading Thread #" << this->id << "]" << "Thread Received " << pathList.size() << "Files";
		for (int i = 0; i < pathList.size(); i++)
			emit processHtml(this->id, Html(pathList[i]), pathList.at(i));

		emit finished();
	}
};

 


class Load : public QObject
{
	Q_OBJECT
private:
	List<Html> *pHtmlList;
	QString htmlFolder;
	QString dictFolder;

signals:
	void finished();
	void loadedHtml(unsigned int, Html, QString);

public:
	Load(List<Html> &htmlList, const QString &htmlFolder, const QString &dictFolder)
	{
		this->pHtmlList = &htmlList;
		this->htmlFolder = htmlFolder;
		this->dictFolder = dictFolder;
	}

public slots:
//TODO::
//This should be different slots and connected by signals 
//when a task finished it sends out signal 
//triggering the next task to start
//The last task sends out loadFinished signal
	void run()
	{
		/* -----Load dictionary----- */
		//TODO:: load dictionary
		/* ---------- */

		/* -----Load htmls----- */
		QDirIterator iter(htmlFolder, QDirIterator::Subdirectories);

		QStringList pathList;
		int i = 0;
		while (iter.hasNext())
		{
			QString path = iter.next();
			if (path.endsWith("."))
				continue;
			pathList.append(path);
			i++;
			if (i == 500)
			{
				QThread *loadHtmlThread = new QThread();
				LoadHtml *loadHtml = new LoadHtml(pathList);
				loadHtml->moveToThread(loadHtmlThread);
				connect(loadHtmlThread, &QThread::started, loadHtml, &LoadHtml::load);
				connect(loadHtml, &LoadHtml::finished, this, &Load::threadFinished);
				connect(loadHtml, &LoadHtml::finished, loadHtml, &QObject::deleteLater);
				connect(loadHtml, &LoadHtml::finished, loadHtmlThread, &QThread::quit);
				connect(loadHtml, &LoadHtml::finished, loadHtmlThread, &QObject::deleteLater);
				connect(loadHtml, &LoadHtml::processHtml, this, &Load::processHtml);

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
			connect(loadHtml, &LoadHtml::finished, this, &Load::threadFinished);
			connect(loadHtml, &LoadHtml::finished, loadHtml, &QObject::deleteLater);
			connect(loadHtml, &LoadHtml::finished, loadHtmlThread, &QThread::quit);
			connect(loadHtml, &LoadHtml::finished, loadHtmlThread, &QObject::deleteLater);
			connect(loadHtml, &LoadHtml::processHtml, this, &Load::processHtml);

			loadHtmlThread->start();
			i = 0;
		}	
		/* ---------- */
	}

	void processHtml(unsigned int threadID,Html html,QString path)
	{
		this->pHtmlList->append(html);
		qDebug() << "[Html Loading Thread # " << threadID << "]" << "Compelete #" << pHtmlList->size() << path << html.getTitle();
		//Transmit the signal to UI thread
		emit loadedHtml(threadID, html, path);
	}

	void threadFinished()
	{
		LoadHtml::threadNum--;
		qDebug() << "[Html Loading Thread]" <<LoadHtml::threadNum << "Thread(s) Remaining...";
		if (LoadHtml::threadNum == 0)
			emit this->finished();
	}
};

