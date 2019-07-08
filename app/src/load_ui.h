#pragma once
#include <QDir>
#include <QTimer>
#include <QTime>
#include <QThread>
#include "ui_load_ui.h"


class LoadUI : public QDialog
{
  Q_OBJECT
private:
  QTime clock;
  QTimer timer;
  // For dragging the window
  QPoint origin;
  bool isPressed;
protected:
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
public:
  LoadUI();
  ~LoadUI();
  static LoadUI *getInstance();
public slots:
  bool loadData();
  void loadingDots();
  void progress(float progress);
signals:
  void start(const QString &databasePath);
private:
  Ui::LoadUI ui;
};

