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
#include "List.h"
#include "BloomFilter.h"
#include "Html.h"
#include "Index.h"
#include "HashMap.h"
#include "HandySearch.h"
#include "LoadThread.h"


/* Initialize the static member */
BloomFilter HandySearch::dictionary;
HashMap<List<Index>*> HandySearch::index;
QStringList HandySearch::sentences;

int main(int argc, char *argv[])
{
	qRegisterMetaType<Html>("Html");
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
	QApplication a(argc, argv);

	HandySearch w;

	return a.exec();
}
