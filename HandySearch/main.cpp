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
	
	//Class tests
	Html html = Html("D:\\Converted\\17.html");
	
	List<Html> list;
	//End of class tests
	
	w.show();
	return a.exec();
}
