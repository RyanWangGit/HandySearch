#include "stdafx.h"
#include "handysearch.h"

using namespace std;

HandySearch::HandySearch(QWidget *parent)
	: QMainWindow(parent)
{
	qRegisterMetaType<Html>("Html");
	ui.setupUi(this);
	//Initialization of application
	//Test of classes
	Load *initialLoadThread = new Load(this->htmlList);
	connect(initialLoadThread, &Load::finished, this, &HandySearch::loadFinished);
	connect(initialLoadThread, &Load::finished, initialLoadThread, &Load::deleteLater);
	initialLoadThread->start();
}

HandySearch::~HandySearch()
{
	
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
