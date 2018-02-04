#include "stdafx.h"
#include "handy_search.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<QList<Html*>>("QList<Html*>");
    qRegisterMetaType<QList<Webpage*>>("QList<Webpage*>");
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));

    HandySearch w;
    /* Start loading and check the directory */
    if (!w.load())
        return 1;

    return a.exec();
}
