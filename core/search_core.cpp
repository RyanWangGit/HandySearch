#include "search_core.h"

SearchCore::SearchCore(const QString &dictionary, const QString &database)
{
    this->setPath(dictionary, database);
    this->hasLoaded = false;
}


SearchCore::SearchCore(const QString &database)
{
    this->setPath(database);
    this->hasLoaded = false;
}


SearchCore::SearchCore()
{
    this->hasLoaded = false;
}


SearchCore::setPath(const QString &database)
{
    this->dictionaryPath = "qrc:/assets/dictionary.txt";
    this->databasePath = database;
}


SearchCore::setPath(const QString &dictionary, const QString &database)
{
    this->dictionaryPath = dictionary;
    this->databasePath = database;
}


void SearchCore::query(const QString &sentence)
{
    if(!this->hasLoaded)
        qFatal("Core hasn't loaded anything yet.");

    QStringList keywords;
    QList<Webpage *> webpages;
    // TODO: implement query
    emit this->result(keywords, webpages);
    return;
}


void SearchCore::load()
{
    this->hasLoaded = true;
}
