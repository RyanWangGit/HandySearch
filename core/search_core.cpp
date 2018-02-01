#include "search_core.h"

SearchCore::SearchCore(const QString &dictionary, const QString &database)
{
    this->hasLoaded = false;
}



QSharedPointer<SearchResult> SearchCore::query(const QString &sentence)
{
    if(!this->hasLoaded)
        qFatal("Core hasn't loaded anything yet.");

    QSharedPointer<SearchResult> result(new SearchResult());

    return result;
}

void SearchCore::load()
{
    this->hasLoaded = true;
}
