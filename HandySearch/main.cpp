/*************************************
 * Copyright(C),2015-2016,Ryan Wang 
 * 
 * File:	main.cpp
 *
 * Version: V1.0
 * 
 * Brief:	This is the top-layer of the application which
 * creates main window of QT and start the application's main
 * event loop.
 *
 * Author:	Ryan
 
 * Date:	Oct. 2015
*************************************/
#include "stdafx.h"
#include "HandySearch.h"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	qRegisterMetaType<QList<Html*>>("QList<Html*>");
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));

	HandySearch w;
	/* Start loading and check the directory */
	if (!w.load())
		return 1;

	return a.exec();
}
