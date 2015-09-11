#include "stdafx.h"

/* Initialize the static member */
unsigned int LoadHtml::threadNum = 0;
BloomFilter HandySearch::dictionary;
HashMap<List<Index>*> HandySearch::index;

int main(int argc, char *argv[])
{
	qRegisterMetaType<Html>("Html");
	QApplication a(argc, argv);
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));

	Loading l;
	HandySearch w;
	QObject::connect(&l, &Loading::finished, &w, &HandySearch::show);
	
	return a.exec();
}
