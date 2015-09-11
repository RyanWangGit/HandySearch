#ifndef HANDYSEARCH_H
#define HANDYSEARCH_H

#include "ui_handysearch.h"

class HandySearch : public QMainWindow
{
	Q_OBJECT
public:
	static BloomFilter dictionary;
	static HashMap<List<Index>*> index;
public slots:
	void onCloseButtonClick();
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
