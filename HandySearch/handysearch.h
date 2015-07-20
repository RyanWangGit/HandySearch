#ifndef HANDYSEARCH_H
#define HANDYSEARCH_H

#include <QtWidgets/QMainWindow>
#include "ui_handysearch.h"

class HandySearch : public QMainWindow
{
	Q_OBJECT
private:
	List<Html> htmlList;
public slots:
	void onCloseButtonClick();
	void loadFinished();
public:
	HandySearch(QWidget *parent = 0);
	~HandySearch();
	//Override paintEvent
	void paintEvent(QPaintEvent *event);

private:
	Ui::HandySearchClass ui;
};

#endif // HANDYSEARCH_H
