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
	BloomFilter *pDict;
	QString htmlFolder;
	QString dictFolder;

signals:
	void loadFinished();
	//Html Thread Signals
	void htmlLoaded(unsigned int, Html, QString);
	void htmlLoadStarted();
	void htmlLoadFinished();
	//Dictionary Thread Signals
	void dictLoaded(int num);
	void dictLoadStarted();
	void dictLoadFinished();
public:
	Load(List<Html> &htmlList, const QString &htmlFolder,BloomFilter &dict, const QString &dictFolder)
	{
		this->pDict = &dict;
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
				pDict->addItem(temp);
				if (index % 1000 == 0)
					emit dictLoaded(1000);
			}
			file.close();
		}
		emit dictLoadFinished();
		/* ---------- */
		
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
			connect(loadHtml, &LoadHtml::finished, this, &Load::htmlThreadFinished);
			connect(loadHtml, &LoadHtml::finished, loadHtml, &QObject::deleteLater);
			connect(loadHtml, &LoadHtml::finished, loadHtmlThread, &QThread::quit);
			connect(loadHtmlThread, &QThread::finished, loadHtmlThread, &QObject::deleteLater);
			connect(loadHtml, &LoadHtml::processHtml, this, &Load::processHtml);

			loadHtmlThread->start();
			i = 0;
		}	
		/* ---------- */
	}

	void processHtml(unsigned int threadID,Html html,QString path)
	{
		//Process Html and set up index
		this->pHtmlList->append(html);

		qDebug() << "[Html Loading Thread # " << threadID << "]" << "Compelete #" << pHtmlList->size() << path << html.getTitle();
		//Transmit the signal to UI thread
		emit htmlLoaded(threadID, html, path);
	}

	void htmlThreadFinished()
	{
		LoadHtml::threadNum--;
		qDebug() << "[Html Loading Thread]" <<LoadHtml::threadNum << "Thread(s) Remaining...";
		if (LoadHtml::threadNum == 0)
		{
			emit this->htmlLoadFinished();
			emit this->loadFinished();
		}
	}
};

