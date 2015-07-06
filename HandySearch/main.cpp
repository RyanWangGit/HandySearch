#include "stdafx.h"
#include "handysearch.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	HandySearch w;
	w.show();
	return a.exec();
}
