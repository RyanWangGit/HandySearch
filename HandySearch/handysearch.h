#ifndef HANDYSEARCH_H
#define HANDYSEARCH_H

#include "ui_handysearch.h"

class HandySearch : public QMainWindow
{
	Q_OBJECT
private:
	bool isLoading;
	List<Html> htmlList;
	BloomFilter dictionary;
	HashMap<QString, List<Index>> index;
	QString htmlFolder;
	QString dictFolder;
	QTime clock;
public slots:
	void onCloseButtonClick();
	/* Load slots */
	void loadFinished();
	//Html load slots
	void htmlLoaded(unsigned int threadID, Html html, QString path);
	void htmlLoadStarted();
	void htmlLoadFinished();
	//Dictionary load slots
	void dictLoaded(int num);
	void dictLoadStarted();
	void dictLoadFinished();
public:
	HandySearch(QWidget *parent = 0);
	//Override paintEvent
	void paintEvent(QPaintEvent *event);

private:
	Ui::HandySearchClass ui;
};

#endif // HANDYSEARCH_H
