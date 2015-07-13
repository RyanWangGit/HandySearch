#include "stdafx.h"
#include "handysearch.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	HandySearch w;
	w.setWindowIconText("HandySearch");
	w.setWindowFlags(Qt::FramelessWindowHint);
	w.setAttribute(Qt::WA_TranslucentBackground);
	
	Html* html = new Html("D:\\Converted\\17.html");
	
	w.show();
	return a.exec();
}
