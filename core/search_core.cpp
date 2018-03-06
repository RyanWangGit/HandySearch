#include "search_core.h"

SearchCore::SearchCore(const QString &dictionary, const QString &database)
{
    this->dictionaryPath = dictionary;
    this->databasePath = database;
    this->hasLoaded = false;
}


SearchCore::SearchCore(const QString &database)
{
    this->dictionaryPath = "qrc:/assets/dictionary.txt";
    this->databasePath = database;
    this->hasLoaded = false;
}


void SearchCore::query(const QString &sentence)
{
    if(!this->hasLoaded)
        qFatal("Core hasn't loaded anything yet.");

    QStringList keywords;
    QList<Webpage *> webpages;


    return;
}


void SearchCore::load()
{
    this->hasLoaded = true;
}
