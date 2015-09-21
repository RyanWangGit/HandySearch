#ifndef HANDYSEARCH_H
#define HANDYSEARCH_H

#include "ui_handysearch.h"

class HandySearch : public QMainWindow
{
#define SHADOWWIDTH 10
#define MINWIDTH 850
#define MINHEIGHT 500
	Q_OBJECT
public:
	static QStringList sentences;
	static BloomFilter dictionary;
	static HashMap<List<Index>*> index;
public slots:
	void loadFinished();
	void onCloseButtonClick();
	//UI slots
	void test();
	void search();
	void textChanged();
	void anchorClicked(const QUrl& url);
public:
	HandySearch(QWidget *parent = 0);
	//Override paintEvent
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

private:
	bool isResultShown;
	void setDefaultUILayout();
	void setShownUILayout();
	void showResult(List<Index*> &resultList, QStringList &wordList);
	QCompleter* completer;
	Ui::HandySearchClass ui;
};

#endif // HANDYSEARCH_H
