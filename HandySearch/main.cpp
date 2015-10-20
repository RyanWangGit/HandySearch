#include "stdafx.h"
#include "List.h"
#include "BloomFilter.h"
#include "Html.h"
#include "Index.h"
#include "HashMap.h"
#include "HandySearch.h"
#include "LoadThread.h"


/* Initialize the static member */
unsigned int LoadHtml::threadNum = 0;
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
