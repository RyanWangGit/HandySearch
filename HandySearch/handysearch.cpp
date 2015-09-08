#include "stdafx.h"
#include "handysearch.h"

using namespace std;

/* Initialize the static member */
unsigned int LoadHtml::threadNum = 0;
BloomFilter HandySearch::dictionary;
HashMap<List<Index>*> HandySearch::index;

HandySearch::HandySearch(QWidget *parent)
	: QMainWindow(parent)
{
	qRegisterMetaType<Html>("Html");
	ui.setupUi(this);

	//Start the application clock
	clock.start();

	//The default html library path
	this->htmlFolder = "C:/Html Library";

	//The default dictionary library path
	this->dictFolder = "C:/Dictionary Library";

	//Set loading status
	this->isLoading = true;

	//Initialization of application
	QThread *initialLoadThread = new QThread();
	Load *initialLoad = new Load(htmlFolder, dictFolder);
	initialLoad->moveToThread(initialLoadThread);
	connect(initialLoadThread, &QThread::started, initialLoad, &Load::run);
	connect(initialLoad, &Load::loadFinished, this, &HandySearch::loadFinished);
	connect(initialLoad, &Load::loadFinished, initialLoad, &QObject::deleteLater);
	connect(initialLoad, &Load::loadFinished, initialLoad, &QObject::deleteLater);
	connect(initialLoadThread, &QThread::finished, initialLoadThread, &QObject::deleteLater);

	//Connect the thread signals to the UI slots
	/* Loading procedure */
	connect(initialLoad, &Load::htmlLoaded, this, &HandySearch::htmlLoaded);
	connect(initialLoad, &Load::htmlLoadStarted, this, &HandySearch::htmlLoadStarted);
	connect(initialLoad, &Load::htmlLoadFinished, this, &HandySearch::htmlLoadFinished);
	connect(initialLoad, &Load::dictLoaded, this, &HandySearch::dictLoaded);
	connect(initialLoad, &Load::dictLoadStarted, this, &HandySearch::dictLoadStarted);
	connect(initialLoad, &Load::dictLoadFinished, this, &HandySearch::dictLoadFinished);
	initialLoadThread->start();

}


void HandySearch::htmlLoadStarted()
{
	QDir dir(this->htmlFolder);
	this->ui.progressBar->setRange(0, dir.entryList().size());
	this->ui.progressBar->setVisible(true);
	this->ui.statusBar->showMessage("   Started Loading Html Library");
	this->ui.statusBar->setStyleSheet("background:rgb(104,33,122);color:rgb(255,255,255)");
	
}

void HandySearch::htmlLoaded(unsigned int threadID, QString path)
{
	this->ui.progressBar->setValue(this->ui.progressBar->value() + 1);
	int currentProgress = this->ui.progressBar->value();
	QString msg = "   Loading Html Library... (";
	msg.append(QString::number(currentProgress));
	msg.append(" / ");
	msg.append(QString::number(this->ui.progressBar->maximum()));
	msg.append(")");
	this->ui.statusBar->showMessage(msg);
	//this->ui.statusBar->setStyleSheet("background:rgb(104,33,122);color:rgb(255,255,255)");
}

void HandySearch::htmlLoadFinished()
{
	this->ui.progressBar->setVisible(false);
	this->ui.statusBar->showMessage("   Ready");
}

void HandySearch::dictLoadStarted()
{
	this->ui.progressBar->setValue(0);
	this->ui.progressBar->setVisible(false);
	this->ui.progressBar->setRange(0, INT_MAX);
	this->ui.statusBar->showMessage("   Started Loading Html Library");
	this->ui.statusBar->setStyleSheet("background:rgb(104,33,122);color:rgb(255,255,255)");
}

void HandySearch::dictLoaded(int num)
{
	this->ui.progressBar->setValue(this->ui.progressBar->value() + num);
	int currentProgress = this->ui.progressBar->value();
	QString msg = "   Loading Dictionary Library... (";
	msg.append(QString::number(currentProgress));
	msg.append(" Items Loaded)");
	this->ui.statusBar->showMessage(msg);
}

void HandySearch::dictLoadFinished()
{
	this->ui.progressBar->setVisible(false);
	this->ui.statusBar->showMessage("   Dictionary Load Finished.");
}


void HandySearch::test()
{
	//qDebug() << this->ui.textEdit->toPlainText();
	WordSegmenter ws(this->ui.textEdit->toPlainText(), this->dictionary);
	QString result;
	QStringList qsl = ws.getResult();
	for (QString word : qsl)
		result.append(word + "/");
	this->ui.textEdit->setPlainText(result);
}

void HandySearch::search()
{
	WordSegmenter ws(this->ui.textEdit->toPlainText(), this->dictionary);
	QString result;
	QStringList qsl = ws.getResult();
	for (QString word : qsl)
	{
		List<Index>* indexList = nullptr;
		List<Index>** pIndexList = HandySearch::index.get(word);
		if (pIndexList == nullptr)
			continue;
		else
			indexList = *pIndexList;

		for (int i = 0; i < indexList->size(); i++)
			result.append(indexList->get(i).getHtml()->getTitle() + "\n");
	}
	this->ui.textEdit->setPlainText(result);
}

void HandySearch::paintEvent(QPaintEvent *event)
{
	int shadowWidth = 5;
	QPainterPath path;
	path.setFillRule(Qt::WindingFill);
	path.addRect(
		shadowWidth,
		shadowWidth,
		this->width() - shadowWidth * 2,
		this->height() - shadowWidth * 2
		);
	
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.fillPath(path, QBrush(Qt::white));

	QColor color(0, 0, 0, 50);
	for (int i = 0; i < shadowWidth * 2; i++)
	{
		QPainterPath path;
		path.setFillRule(Qt::WindingFill);

		path.addRect(
			shadowWidth - i, 
			shadowWidth - i, 
			this->width() - (shadowWidth - i) * 2, 
			this->height() - (shadowWidth - i) * 2
			);

		color.setAlpha(150 - qSqrt(i) * 50);
		painter.setPen(color);
		painter.drawPath(path);
	}
}

/* ------Slot functions--------- */
void HandySearch::loadFinished()
{
	this->isLoading = false;
	qDebug() << "Time elapsed: " << clock.elapsed() << "with List size:" << Html::totalNum;
}
//This is a test method
void HandySearch::onCloseButtonClick()
{
	try
	{
		this->close();
	}	
	catch (...)
	{
		qDebug() << "Shutdown Error";
	}
		
	
}
