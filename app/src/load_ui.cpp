#include <cmath>
#include <QMessageBox>
#include <QFileDialog>
#include <QPropertyAnimation>
#include "load_ui.h"
#include "handy_search.h"


LoadUI::LoadUI()
{
  ui.setupUi(this);
  isPressed = false;

  connect(ui.close, &QPushButton::clicked, []{ QApplication::quit(); });

  setWindowIconText("Handy Search");
  setWindowFlags(Qt::FramelessWindowHint);
  setModal(false);
}


void LoadUI::progress(double progress)
{
  this->ui.statusBar->setText("Loading - " + QString::number(progress * 100, 'g', 4) + "%");
}


bool LoadUI::loadData()
{
  QString databasePath;

  databasePath = QFileDialog::getOpenFileName(this, "Choose Webpage database", "", "Database (*.db *.sqlite *.sqlite3)");
  if (databasePath.isEmpty())
  {
    QApplication::beep();
    if (QMessageBox::question(nullptr, "Warning", "Are you sure you want to quit the application?") == QMessageBox::Yes)
      return false;
  }
  emit start(databasePath);

  connect(&timer, &QTimer::timeout, this, &LoadUI::loadingDots);
  timer.start(20);

  QPropertyAnimation geometry(ui.statusBar, "geometry");
  geometry.setDuration(1000);
  QPoint leftTop = ui.statusBar->pos();
  QPoint rightBot(leftTop.x() + ui.statusBar->width(), leftTop.y() + ui.statusBar->height());
  QRect rect(leftTop, rightBot);
  geometry.setStartValue(rect);
  rect.setY(rect.y() - 15);
  geometry.setEndValue(rect);
  geometry.start();

  return true;
}


void LoadUI::loadingDots()
{
  //TODO:
  //The implementaion seems stupid and needs to be improved.
  //To be an QWidget
  static int time1 = 0;
  static int time2 = -2;
  static int time3 = -4;
  static int time4 = -6;
  static int time5 = -8;
  QPoint point1 = ui.dot1->pos();
  QPoint point2 = ui.dot2->pos();
  QPoint point3 = ui.dot3->pos();
  QPoint point4 = ui.dot4->pos();
  QPoint point5 = ui.dot5->pos();

  point1.rx() = pow(time1 - 25, 3) * 0.0069 + 200;
  if (point1.rx() >= 350)
    point1.rx() = 50;
  point2.rx() = pow(time2 - 25, 3) * 0.0069 + 194;
  if (point2.rx() >= 344)
    point2.rx() = 44;
  point3.rx() = pow(time3 - 25, 3) * 0.0069 + 188;
  if (point3.rx() >= 338)
    point3.rx() = 38;
  point4.rx() = pow(time4 - 25, 3) * 0.0069 + 182;
  if (point4.rx() >= 332)
    point4.rx() = 32;
  point5.rx() = pow(time5 - 25, 3) * 0.0069 + 176;
  if (point5.rx() >= 326)
    point5.rx() = 26;
  ui.dot1->move(point1);
  ui.dot2->move(point2);
  ui.dot3->move(point3);
  ui.dot4->move(point4);
  ui.dot5->move(point5);

  time1++;
  time2++;
  time3++;
  time4++;
  time5++;
  time1 %= 50;
  time2 %= 50;
  time3 %= 50;
  time4 %= 50;
  time5 %= 50;
}


void LoadUI::mousePressEvent(QMouseEvent *event)
{
  isPressed = true;
  origin = event->pos();
}


void LoadUI::mouseMoveEvent(QMouseEvent *event)
{
  if (isPressed)
    move(event->globalX() - origin.x(), event->globalY() - origin.y());
}


void LoadUI::mouseReleaseEvent(QMouseEvent * event)
{
  isPressed = false;
}


LoadUI::~LoadUI()
{
}
