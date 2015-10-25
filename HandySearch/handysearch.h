#ifndef HANDYSEARCH_H
#define HANDYSEARCH_H

#include "ui_handysearch.h"
#include "List.h"
#include "BloomFilter.h"
#include "HashMap.h"
#include "Index.h"

/**
 * Class:	HandySearch
 *
 * Brief:	Main QT class,manages all threads and global variables,posseses UI thread.
 *
 * Date:	Oct. 2015
 */
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
	void loadCanceled();
	void loadFinished();
	void onCloseButtonClick();
	//UI slots
	void segment();
	void search();
	void anchorClicked(const QUrl& url);
	void about();
public:
	HandySearch(QWidget *parent = 0);
	void resizeEvent(QResizeEvent *event);

private:
	QTime clock;
	bool isResultShown;
	void putInContentList(Index* index, List<Index *>& list);
	void putInTitleList(Index* index, List<Index *>& list);
	void setDefaultUILayout();
	void setResultUILayout();
	void showResult(List<Index*> &resultList, QStringList &wordList);
	QCompleter* completer;
	Ui::HandySearchClass ui;
};

#endif // HANDYSEARCH_H
