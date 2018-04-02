#pragma once

#include "ui_handy_search.h"
#include "bloom_filter.h"
#include "load_ui.h"
#include "search_core.h"


class HandySearch : public QMainWindow
{
    Q_OBJECT
public slots:
    void loadCanceled();
    void loadFinished();
    /* UI slots */
    void segment();
    void search();
    void searchResult(const QList<Webpage*> &resultList, const QStringList &keyWordList);
public:
    HandySearch(QWidget *parent = 0);
    Dictionary *getDictionary();
    InvertedList *getInvertedList();
    bool load();
protected:
    void resizeEvent(QResizeEvent *event);
private:
    QTime clock;
    bool isResultShown;
    void setDefaultUILayout();
    void setResultUILayout();
    void showResult(const QList<Webpage*> &resultList, const QStringList &wordList);
    QCompleter* completer;
    SearchCore core;
    QThread coreThread;
    LoadUI loadUI;
    Ui::HandySearchClass ui;
    /* Constants */
    const int MINWIDTH;
    const int MINHEIGHT;
};

