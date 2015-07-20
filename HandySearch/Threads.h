#pragma once

class LoadHtml : public QThread
{
	Q_OBJECT
private :
	QStringList pathList;
public:
	LoadHtml(const QStringList &pathList)
	{
		this->pathList = pathList;
	}
	void run() Q_DECL_OVERRIDE
	{
		qDebug() << "[Html Loading Thread]" << "Thread Received " << pathList.size() << "Files";
		for (int i = 0; i < pathList.size(); i++)
			emit processHtml(Html(pathList[i]), pathList.at(i));
	}
signals:
	void processHtml(Html,QString);
};

class Load : public QThread
{
	Q_OBJECT
private:
	List<Html> *pHtmlList;
	int threadNum;
public:
	Load(List<Html> &htmlList)
	{
		this->threadNum = 0;
		this->pHtmlList = &htmlList;
	}

	void run() Q_DECL_OVERRIDE
	{
		QTime time;
		time.start();
		QDirIterator iter("D:/Html Library", QDirIterator::Subdirectories);

		QStringList pathList;
		int i = 0;
		while (iter.hasNext())
		{
			QString path = iter.next();
			if (path.endsWith("."))
				continue;
			pathList.append(path);
			i++;
			if (i == 1000)
			{
				threadNum++;
				LoadHtml *loadHtmlThread = new LoadHtml(pathList);
				connect(loadHtmlThread, &LoadHtml::finished, this, &Load::threadFinished);
				connect(loadHtmlThread, &QThread::finished, loadHtmlThread, &QObject::deleteLater);

				connect(loadHtmlThread, &LoadHtml::processHtml, this, &Load::processHtml);
				loadHtmlThread->start();
				pathList.clear();
				i = 0;
			}
		}
		//The remaining
		if (pathList.size() != 0)
		{
			threadNum++;
			LoadHtml *loadHtmlThread = new LoadHtml(pathList);
			connect(loadHtmlThread, &LoadHtml::finished, this, &Load::threadFinished);
			connect(loadHtmlThread, &QThread::finished, loadHtmlThread, &QObject::deleteLater);

			connect(loadHtmlThread, &LoadHtml::processHtml, this, &Load::processHtml);
			loadHtmlThread->start();
			pathList.clear();
			i = 0;
		}
		while (this->threadNum != 0)
		{
			this->msleep(100);
		}
		qDebug() << "[Html Loading Thread]" << "Time elapsed : " << time.elapsed();
		qDebug() << "[Html Loading Thread]" << "List size:" << this->pHtmlList->size();
	}
public slots:
	void processHtml(Html html,QString path)
	{
		this->pHtmlList->append(html);
		qDebug() << "[Html Loading Thread]" << "Compelet #" << pHtmlList->size() << path << (pHtmlList->get(pHtmlList->size() - 1).getTitle());
	}

	void threadFinished()
	{
		this->threadNum--;
		qDebug() << "[Html Loading Thread]" << this->threadNum << "Thread(s) Remaining...";
	}
};

