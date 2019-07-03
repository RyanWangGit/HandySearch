#pragma once

#include "ui_handy_search.h"
#include "bloom_filter.h"
#include "load_ui.h"
#include "search_core.h"


class HandySearch : public QMainWindow
{
  Q_OBJECT
public:
  HandySearch(QWidget *parent = 0);
  void searchResult(const QStringList &keywords, const QList<Webpage> &webpages);
  bool load();
signals:
  void startLoading(int from=1);
  void query(const QString &sentence);
  void progress(const QString &hint, float progress);
  void loadFinished();
protected:
  void resizeEvent(QResizeEvent *event);
private:
  QTime clock;
  bool isResultShown;
  void setDefaultUILayout();
  void setResultUILayout();
  void showResult(const QStringList &keywords, const QList<Webpage> &webpages);
  QCompleter* completer;
  SearchCore core;
  QThread coreThread;
  void handleFinished();

  LoadUI loadUI;
  Ui::HandySearchClass ui;
  /* Constants */
  const int MINWIDTH;
  const int MINHEIGHT;
};

