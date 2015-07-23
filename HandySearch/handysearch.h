#ifndef HANDYSEARCH_H
#define HANDYSEARCH_H

#include "ui_handysearch.h"

class HandySearch : public QMainWindow
{
	Q_OBJECT
private:
	List<Html> htmlList;
	BloomFilter dictionary;
	QTime clock;
public slots:
	void onCloseButtonClick();
	void loadFinished();
	void processHtml(unsigned int threadID, Html html, QString path);
public:
	HandySearch(QWidget *parent = 0);
	~HandySearch();
	//Override paintEvent
	void paintEvent(QPaintEvent *event);

private:
	Ui::HandySearchClass ui;
};

#endif // HANDYSEARCH_H
