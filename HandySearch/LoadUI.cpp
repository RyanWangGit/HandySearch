#include "stdafx.h"
//#define DEBUG

LoadUI::LoadUI()
{
	ui.setupUi(this);
	this->ui.close->installEventFilter(this);
	this->setWindowIconText("Handy Search");
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->show();

	this->currentProgress = 0;
	this->maximumProgress = 0;

	//Start the application clock
	clock.start();
	QString currentPath = QApplication::applicationDirPath();
	//The default html library path
	this->htmlFolder = currentPath + "/Html Library";

	//The default dictionary library path
	this->dictFolder = currentPath + "/Dictionary Library";

	if (!this->htmlFolder.exists() || !this->dictFolder.exists())
	{
		while (!this->dictFolder.exists())
		{
			this->dictFolder = QFileDialog::getExistingDirectory(this, "Choose Dictionary Library", "");
			if (!this->dictFolder.exists())
			{
				QApplication::beep();
				if (QMessageBox::question(nullptr, "Warning", "Are you sure you want to quit the application?") == QMessageBox::Yes)
				{
					this->close();
					return;
				}
			}
		}
		while (!this->htmlFolder.exists())
		{
			this->htmlFolder = QFileDialog::getExistingDirectory(this, "Choose Html Library", "");
			if (!this->htmlFolder.exists())
			{
				QApplication::beep();
				if (QMessageBox::question(nullptr, "Warning", "Are you sure you want to quit the application?") == QMessageBox::Yes)
				{
					this->close();
					return;
				}
			}
		}
		
	}

	this->isPressed = false;
	
	//Initialization of application
	QThread *initialLoadThread = new QThread();
	Load *initialLoad = new Load(htmlFolder, dictFolder);
	initialLoad->moveToThread(initialLoadThread);
	connect(initialLoadThread, &QThread::started, initialLoad, &Load::run);
	connect(initialLoad, &Load::loadFinished, initialLoad, &QObject::deleteLater);
	connect(initialLoad, &Load::loadFinished, initialLoadThread, &QThread::quit);
	connect(initialLoadThread, &QThread::finished, initialLoadThread, &QObject::deleteLater);

	//Connect the thread signals to the UI slots
	/* Loading procedure */
	connect(initialLoad, &Load::loadStarted, this, &LoadUI::loadStarted);
	connect(initialLoad, &Load::loadFinished, this, &LoadUI::loadFinished);
	connect(initialLoad, &Load::htmlLoaded, this, &LoadUI::htmlLoaded);
	connect(initialLoad, &Load::htmlLoadStarted, this, &LoadUI::htmlLoadStarted);
	connect(initialLoad, &Load::htmlLoadFinished, this, &LoadUI::htmlLoadFinished);
	connect(initialLoad, &Load::dictLoaded, this, &LoadUI::dictLoaded);
	connect(initialLoad, &Load::dictLoadStarted, this, &LoadUI::dictLoadStarted);
	connect(initialLoad, &Load::dictLoadFinished, this, &LoadUI::dictLoadFinished);
	initialLoadThread->start();
}

/* ------Slot functions--------- */
void LoadUI::loadingDots()
{
	static int time1 = 0;
	static int time2 = -2;
	static int time3 = -4;
	static int time4 = -6;
	static int time5 = -8;
	QPoint point1 = this->ui.dot1->pos();
	QPoint point2 = this->ui.dot2->pos();
	QPoint point3 = this->ui.dot3->pos();
	QPoint point4 = this->ui.dot4->pos();
	QPoint point5 = this->ui.dot5->pos();

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
	this->ui.dot1->move(point1);
	this->ui.dot2->move(point2);
	this->ui.dot3->move(point3);
	this->ui.dot4->move(point4);
	this->ui.dot5->move(point5);

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


void LoadUI::htmlLoadStarted()
{
	QDir dir(this->htmlFolder);
	this->currentProgress = 0;
	this->maximumProgress = dir.entryList().size();
	this->ui.statusBar->setText("Started Loading Html Library");
}

void LoadUI::htmlLoaded(unsigned int threadID, QString path)
{
	this->currentProgress++;
	QString msg ;
	int percent = ((float)this->currentProgress / this->maximumProgress) * 100;
	msg.append(QString::number(percent));
	msg.append(" % - Loading Htmls");
	this->ui.statusBar->setText(msg);
}

void LoadUI::htmlLoadFinished()
{
	this->ui.statusBar->setText("Ready");
}

void LoadUI::dictLoadStarted()
{
	//Started Loading Dictionary Library
	this->currentProgress = 0;
	this->maximumProgress = 0;
}

void LoadUI::dictLoaded(int num)
{
	this->currentProgress += num;
	QString msg;
	msg.append(QString::number(currentProgress));
	msg.append(" Items Loaded");
	this->ui.statusBar->setText(msg);
}

void LoadUI::dictLoadFinished()
{
	//Dictionary Load Finished
}


void LoadUI::loadStarted()
{
	connect(&timer, &QTimer::timeout, this, &LoadUI::loadingDots);
	timer.start(20);

	QPropertyAnimation *geometry = new QPropertyAnimation(this->ui.statusBar, "geometry");
	geometry->setDuration(1000);
	QPoint leftTop = this->ui.statusBar->pos();
	QPoint rightBot(leftTop.x() + this->ui.statusBar->width(), leftTop.y() + this->ui.statusBar->height());
	QRect rect(leftTop, rightBot);
	geometry->setStartValue(rect);
	rect.setY(rect.y() - 15);
	geometry->setEndValue(rect);
	geometry->start();
}

void LoadUI::loadFinished()
{
#ifdef DEBUG
	QMessageBox::information(nullptr, "time", "Time elapsed: " + QString::number(clock.elapsed()));
	qDebug() << "Time elapsed: " << clock.elapsed() << "with List size:" << Html::totalNum;
#endif
	emit finished();
	this->close();
}


void LoadUI::mousePressEvent(QMouseEvent *event)
{
	this->isPressed = true;
	this->origin = event->pos();
}

void LoadUI::mouseMoveEvent(QMouseEvent *event)
{
	if (this->isPressed)
		this->move(event->globalX() - this->origin.x(), event->globalY() - this->origin.y());
	if (this->ui.close->rect().contains(event->pos()))
		this->ui.close->setPixmap(QPixmap(QString::fromUtf8(":/Resources/Buttons/CloseOver.png")));
}


void LoadUI::mouseReleaseEvent(QMouseEvent * event)
{
	this->isPressed = false;
}

bool LoadUI::eventFilter(QObject *obj, QEvent *event)
{
	if (obj == this->ui.close)
	{
		switch (event->type())
		{
		case QEvent::Enter:
			this->ui.close->setPixmap(QPixmap(QString::fromUtf8(":/Resources/Buttons/CloseOver.png")));
			break;
		case QEvent::Leave:
			this->ui.close->setPixmap(QPixmap(QString::fromUtf8(":/Resources/Buttons/CloseNormalW.png")));
			break;
		case QEvent::MouseButtonPress:
			this->close();
			break;
		default:
			break;
		}
	}

	return QWidget::eventFilter(obj, event);
}

LoadUI::~LoadUI()
{
}
