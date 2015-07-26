#include "stdafx.h"
#include "handysearch.h"
#include <QtWidgets/QApplication>



int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	HandySearch w;
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
	w.setWindowIconText("HandySearch");
	w.setWindowFlags(Qt::FramelessWindowHint);
	w.setAttribute(Qt::WA_TranslucentBackground);
	w.show();
	return a.exec();
}
