#include "search_core.h"

SearchCore::SearchCore(const QString &dictionary, const QString &database)
{
    this->hasLoaded = false;
}



void SearchCore::query(const QString &sentence)
{
    if(!this->hasLoaded)
        qFatal("Core hasn't loaded anything yet.");

    QStringList keywords;
    QList<Webpage *> webpages;


    emit this->result(keywords, webpages);
    return;
}

void SearchCore::load()
{
    this->hasLoaded = true;
}
