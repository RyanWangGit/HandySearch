/*************************************
 * Copyright(C),2015-2016,Ryan Wang 
 * 
 * File:	LoadUI.cpp
 *
 * Version: V1.0
 * 
 * Brief:	This implements the user interface of loading dialog,supporting
 * multiple animations and information-displaying.
 *
 * Author:	Ryan
 
 * Date:	Oct. 2015
*************************************/
#include "stdafx.h"
#include "LoadUI.h"
#include "HandySearch.h"

/* Initialization of static members */
LoadUI* LoadUI::instance = nullptr;

/*--------------------------
* LoadUI::LoadUI
* 	Loading dialog constructor.
----------------------------*/
LoadUI::LoadUI()
{
	ui.setupUi(this);
	/* Initialize variables */
	isPressed = false;
	currentProgress = 0;
	maximumProgress = 0;
	instance = &(*this);

	/* Bind the signal */
	connect(ui.close, &QPushButton::clicked, this, &LoadUI::canceled);

	setWindowIconText("Handy Search");
	setWindowFlags(Qt::FramelessWindowHint);
}


/*--------------------------
* LoadUI::checkDirectory
* 	Check if current directory is correct or not,and queries for right directory if not.
* Returns:	Whether the user decides to quit the appilication or not.
----------------------------*/
bool LoadUI::checkDirectory()
{
	QString currentPath = QApplication::applicationDirPath();
	/* The default html library path */
	htmlFolder = currentPath + "/Html Library";

	/* The default dictionary library path */
	dictFolder = currentPath + "/Dictionary Library";

	/* If html folder or dictionary folder doesn't exist */
	if (!htmlFolder.exists() || !dictFolder.exists())
	{
		while (!dictFolder.exists())
		{
			dictFolder = QFileDialog::getExistingDirectory(this, "Choose Dictionary Library", "");
			if (!dictFolder.exists())
			{
				QApplication::beep();
				if (QMessageBox::question(nullptr, "Warning", "Are you sure you want to quit the application?") == QMessageBox::Yes)
				{
					emit canceled();
					return false;
				}
			}
		}
		while (!htmlFolder.exists())
		{
			htmlFolder = QFileDialog::getExistingDirectory(this, "Choose Html Library", "");
			if (!htmlFolder.exists())
			{
				QApplication::beep();
				if (QMessageBox::question(nullptr, "Warning", "Are you sure you want to quit the application?") == QMessageBox::Yes)
				{
					emit canceled();
					return false;
				}
			}
		}
	}
	return true;
}


/*--------------------------
* LoadUI::loadData
* 	Start loading htmls and dictionary,connect the signals and slots,
* and start initialization thread.
----------------------------*/
void LoadUI::loadData()
{
	/* Show up the dialog */
	show();
	/* Check the directory is correct or not */
	if (!checkDirectory())
		return;

	/* Start the loading clock */
	clock.start();
	
	/* Set dictionary folder and connect UI signals */
	Dictionary* dict = HandySearch::getInstance()->getDictionary();
	dict->setDictFolder(dictFolder);
	connect(dict, &Dictionary::dictLoadStarted, this, &LoadUI::dictLoadStarted);
	connect(dict, &Dictionary::dictLoaded, this, &LoadUI::dictLoaded);
	connect(dict, &Dictionary::dictLoadFinished, this, &LoadUI::dictLoadFinished);

	/* Set html folder and connect UI signals */
	InvertedList* invertedList = HandySearch::getInstance()->getInvertedList();
	invertedList->setHtmlFolder(htmlFolder);
	connect(invertedList, &InvertedList::htmlLoadStarted, this, &LoadUI::htmlLoadStarted);
	/* htmlLoaded signal is directly connected to LoadUI inside HtmlLoadTask */
	connect(invertedList, &InvertedList::htmlLoadFinished, this, &LoadUI::htmlLoadFinished);
	
	
	/* Connect the loading procedure signals/slots */
	connect(this, &LoadUI::start, this, &LoadUI::loadStarted);
	connect(this, &LoadUI::start, dict, &Dictionary::load);
	connect(dict, &Dictionary::dictLoadFinished, invertedList, &InvertedList::load);
	connect(invertedList, &InvertedList::htmlLoadFinished, this, &LoadUI::loadFinished);

	/* Start loading */
	emit start();

	return;
}


/* ------Slot functions--------- */
/*--------------------------
* LoadUI::loadingDots
* 	Controls the moving trace of five loading dots,provides a fancy loading animation.
----------------------------*/
void LoadUI::loadingDots()
{
	//TODO:
	//The implementaion seems stupid and needs to be improved.
	//To be an QWidget
	static int time1 = 0;
	static int time2 = -2;
	static int time3 = -4;
	static int time4 = -6;
	static int time5 = -8;
	QPoint point1 = ui.dot1->pos();
	QPoint point2 = ui.dot2->pos();
	QPoint point3 = ui.dot3->pos();
	QPoint point4 = ui.dot4->pos();
	QPoint point5 = ui.dot5->pos();

	point1.rx() = pow(time1 - 25, 3) * 0.0069 + 200;
	if (point1.rx() >= 350)
		point1.rx() = 50;
	point2.rx() = pow(time2 - 25, 3) * 0.0069 + 194;
	if (point2.rx() >= 344)
		point2.rx() = 44;
	point3.rx() = pow(time3 - 25, 3) * 0.0069 + 188;
	if (point3.rx() >= 338)
		point3.rx() = 38;
	point4.rx() = pow(time4 - 25, 3) * 0.0069 + 182;
	if (point4.rx() >= 332)
		point4.rx() = 32;
	point5.rx() = pow(time5 - 25, 3) * 0.0069 + 176;
	if (point5.rx() >= 326)
		point5.rx() = 26;
	ui.dot1->move(point1);
	ui.dot2->move(point2);
	ui.dot3->move(point3);
	ui.dot4->move(point4);
	ui.dot5->move(point5);

	time1++;
	time2++;
	time3++;
	time4++;
	time5++;
	time1 %= 50;
	time2 %= 50;
	time3 %= 50;
	time4 %= 50;
	time5 %= 50;
}


/*--------------------------
* LoadUI::htmlLoadStarted
* 	Initialize the variables and get ready to load html.
----------------------------*/
void LoadUI::htmlLoadStarted()
{
	QDir dir(htmlFolder);
	currentProgress = 0;
	maximumProgress = dir.entryList().size();
	ui.statusBar->setText("Started Loading Html Library");
}


/*--------------------------
* LoadUI::htmlLoaded
* 	Receive html loaded signal from sub-threads and set the loading text.
* Parameter:
* 	unsigned int threadID - Thread ID the html was loaded,not currently used.
* 	QString path - Html file path,not currently used.
----------------------------*/
void LoadUI::htmlLoaded(const int num)
{
	currentProgress += num;
	QString msg ;
	int percent = ((float)currentProgress / maximumProgress) * 100;
	msg.append(QString::number(percent));
	msg.append(" % - Loading Htmls");
	ui.statusBar->setText(msg);
}


/*--------------------------
* LoadUI::htmlLoadFinished
* 	Receives html loading finished signal,set loading text and quit loading thread.
----------------------------*/
void LoadUI::htmlLoadFinished()
{
	ui.statusBar->setText("Ready");
	currentProgress = 0;
	maximumProgress = 0;
}


/*--------------------------
* LoadUI::dictLoadStarted
* 	Initialize the variables and get ready to load dictionary.
----------------------------*/
void LoadUI::dictLoadStarted()
{
	/* Started Loading Dictionary Library */
	currentProgress = 0;
	maximumProgress = 0;
}


/*--------------------------
* LoadUI::dictLoaded
* 	Receive html loaded signal and set the loading text.
* Parameter:
* 	int num - Number of dictionary items loaded.
----------------------------*/
void LoadUI::dictLoaded(const int num)
{
	currentProgress += num;
	QString msg;
	msg.append(QString::number(currentProgress));
	msg.append(" Items Loaded");
	ui.statusBar->setText(msg);
}


/*--------------------------
* LoadUI::dictLoadFinished
* 	Receives html loading finished signal,this method is not implemented.
----------------------------*/
void LoadUI::dictLoadFinished()
{
	/* Quit the task-load threads */
	
}


/*--------------------------
* LoadUI::loadStarted
* 	Receives load started signal,loading text fades in,initiate loading dots animation.
----------------------------*/
void LoadUI::loadStarted()
{
	connect(&timer, &QTimer::timeout, this, &LoadUI::loadingDots);
	timer.start(20);

	QPropertyAnimation geometry(ui.statusBar, "geometry");
	geometry.setDuration(1000);
	QPoint leftTop = ui.statusBar->pos();
	QPoint rightBot(leftTop.x() + ui.statusBar->width(), leftTop.y() + ui.statusBar->height());
	QRect rect(leftTop, rightBot);
	geometry.setStartValue(rect);
	rect.setY(rect.y() - 15);
	geometry.setEndValue(rect);
	geometry.start();
}


/*--------------------------
* LoadUI::loadFinished
* 	Recieves load finished signal,close the dialog.
----------------------------*/
void LoadUI::loadFinished()
{
#ifdef _DEBUG
	QMessageBox::information(nullptr, "Time", "Time elapsed: " + QString::number(clock.elapsed()));
	qDebug() << "Time elapsed: " << clock.elapsed() << "with List size:" << Html::getTotalHtmlCount();
#endif
	emit finished();
	/* Close the dialog */
	close();
}


/*--------------------------
* LoadUI::mousePressEvent
* 	Override method to implement drag movement.
* Parameter:
* 	QMouseEvent * event - Mouse event.
----------------------------*/
void LoadUI::mousePressEvent(QMouseEvent *event)
{
	isPressed = true;
	origin = event->pos();
}


/*--------------------------
* LoadUI::mouseMoveEvent
* 	Override method to implement drag movement.
* Parameter:
* 	QMouseEvent * event - Mouse event.
----------------------------*/
void LoadUI::mouseMoveEvent(QMouseEvent *event)
{
	if (isPressed)
		move(event->globalX() - origin.x(), event->globalY() - origin.y());
}


/*--------------------------
* LoadUI::mouseReleaseEvent
* 	Override method to implement drag movement.
* Parameter:
* 	QMouseEvent * event - Mouse event.
----------------------------*/
void LoadUI::mouseReleaseEvent(QMouseEvent * event)
{
	isPressed = false;
}


/*--------------------------
* LoadUI::~LoadUI
* 	Destructor of LoadUI,not implemented.
----------------------------*/
LoadUI::~LoadUI()
{
}

LoadUI* LoadUI::getInstance()
{
	return instance;
}

