#include "stdafx.h"
#include "handysearch.h"

HandySearch::HandySearch(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setWindowIconText("HandySearch");
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground);
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
