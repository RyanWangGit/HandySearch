#include "stdafx.h"
#include "handysearch.h"

using namespace std;

unsigned int LoadHtml::threadNum = 0;

HandySearch::HandySearch(QWidget *parent)
	: QMainWindow(parent)
{
	qRegisterMetaType<Html>("Html");
	ui.setupUi(this);

	//Start the application clock
	clock.start();

	//The default html library path
	QString htmlFolder = "C:/Html Library";
	QDir dir(htmlFolder);
	this->ui.progressBar->setRange(0, dir.entryList().size());

	//The default dictionary library path
	QString dictFolder = "C:/Dictionary Library";

	//Initialization of application
	QThread *initialLoadThread = new QThread();
	Load *initialLoad = new Load(this->htmlList, htmlFolder, dictFolder);
	initialLoad->moveToThread(initialLoadThread);
	connect(initialLoadThread, &QThread::started, initialLoad, &Load::run);
	connect(initialLoad, &Load::finished, this, &HandySearch::loadFinished);
	connect(initialLoad, &Load::finished, initialLoad, &QObject::deleteLater);
	connect(initialLoad, &Load::finished, initialLoadThread, &QThread::quit);
	connect(initialLoadThread, &QThread::finished, initialLoadThread, &QObject::deleteLater);

	//Connect the thread signals to the UI slots
	connect(initialLoad, &Load::loadedHtml, this, &HandySearch::processHtml);
	initialLoadThread->start();

}

HandySearch::~HandySearch()
{
	
}


void HandySearch::processHtml(unsigned int threadID, Html html, QString path)
{
	this->ui.progressBar->setValue(this->ui.progressBar->value() + 1);
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
	qDebug() << "Load finished " << this->htmlList.size();
	qDebug() << "Time elapsed: " << clock.elapsed() << "with List size:" << this->htmlList.size();
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
