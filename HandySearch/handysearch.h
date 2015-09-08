#ifndef HANDYSEARCH_H
#define HANDYSEARCH_H

#include "ui_handysearch.h"

class HandySearch : public QMainWindow
{
	Q_OBJECT
private:
	bool isLoading;
	QString htmlFolder;
	QString dictFolder;
	QTime clock;
public:
	static BloomFilter dictionary;
	static HashMap<List<Index>*> index;
public slots:
	void onCloseButtonClick();
	/* Load slots */
	void loadFinished();
	//Html load slots
	void htmlLoaded(unsigned int threadID, QString path);
	void htmlLoadStarted();
	void htmlLoadFinished();
	//Dictionary load slots
	void dictLoaded(int num);
	void dictLoadStarted();
	void dictLoadFinished();
	//UI slots
	void test();
	void search();
public:
	HandySearch(QWidget *parent = 0);
	//Override paintEvent
	void paintEvent(QPaintEvent *event);

private:
	Ui::HandySearchClass ui;
};

#endif // HANDYSEARCH_H
