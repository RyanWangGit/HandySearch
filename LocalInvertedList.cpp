/*************************************
* Copyright(C),2015-2016,Ryan Wang
*
* File:    LocalInvertedList.cpp
*
* Version: V1.0
*
* Brief:    Local invertedList consists of several html tasks and a local hashmap
* which contains the index of htmls.
*
* Author:    Ryan
*
* Date:    Nov. 2015
*************************************/
#include "stdafx.h"
#include "LocalInvertedList.h"
#include "WordSegmenter.h"
#include "HandySearch.h"


/*--------------------------
* LocalInvertedList::putToInvertedList
*     Analyze the html and do the word segmentation,create index and
* put the index into inverted list.
*
* Parameter:
*     Html * html - The html pointer which is to be operated.
----------------------------*/
void LocalInvertedList::putInLocalList(Html* html)
{
    if (html->hasAnalyzed())
        return;
    else
        html->setAnalyzed(true);

    unsigned int pos = 0;
    
    QStringList wordList = html->getText();

    for (QString word : wordList)
    {
        pos += word.size();
        /* If the first character isn't chinese */
        QChar ch = word.at(0);
        if (!(ch.unicode() >= 0x4e00 && ch.unicode() <= 0x9FA5))
            continue;

        /* Find if there's a list existed */
        List<Index>* indexList = nullptr;
        List<Index>** pIndexList = localHashMap->get(word);
        if (pIndexList == nullptr)
        {
            indexList = new List<Index>();
            localHashMap->put(word, indexList);
            hashMapContent.append(indexList);
        }
        else
            indexList = *pIndexList;

        /* Find if the word belongs to an existed index */
        bool hasFound = false;
        for (int i = 0; i < indexList->size(); i++)
        {
            Index* index = &indexList->get(i);
            /* Yes */
            if (index->getHtml() == html)
            {
                index->getPosition().append(pos);
                hasFound = true;
                break;
            }
        }
        /* No */
        if (!hasFound)
        {
            Html* temp = html;
            indexList->append(*(new Index(temp, pos)));
        }
    }
}

/*--------------------------
* LocalInvertedList::LocalInvertedList
*     Local inverted list constructor.
* Parameter:
*     const QStringList & pathList - Path of files.
----------------------------*/
LocalInvertedList::LocalInvertedList(const QStringList& pathList)
{
    this->localHashMap = new HashMap<List<Index>*>();
    this->pathList = pathList;
}


/*--------------------------
* LocalInvertedList::~LocalInvertedList
*     Local inverted list destructor, delet the local hashmap and the index list it contains.
----------------------------*/
LocalInvertedList::~LocalInvertedList()
{
    /* Delete the content of hashmap */
    for (List<Index>* indexList : hashMapContent)
        delete indexList;

    delete localHashMap;
}

/*--------------------------
* LocalInvertedList::load
*     Load several html files,this is a sub-task,managed by HtmlLoadTask.
----------------------------*/
void LocalInvertedList::localLoadStart()
{
    QStringList titleList;
    const int LOAD_SIGNAL_FREQUENCY = 1;
#ifdef _DEBUG
    qDebug() << "[Html Loading Thread #" << QObject::thread()->currentThreadId() << "]" << "Thread Received " << pathList.size() << "Files";
#endif 
    for (int i = 0; i < pathList.size(); i++)
    {
#ifndef SKIPLOAD 
        QString path = pathList.at(i);
        /* Ignore those aren't html files */
        if (!(path.endsWith(".html") || path.endsWith(".htm")))
            continue;

        Html *pHtml = new Html(path);
        putInLocalList(pHtml);

        /* Remove all pucntuations , both English and Chinese ones */
        QString title = pHtml->getTitle();
        title.replace(QRegularExpression("\\pP|\\pS"), "");
        title.chop(10);
        titleList.append(title);
        
#ifdef _DEBUG
        qDebug() << "[Html Loading Thread # " << QObject::thread()->currentThreadId() << "]" << "Compelete #" << Html::getTotalHtmlCount() << path << pHtml->getTitle();
#endif
        if (i % LOAD_SIGNAL_FREQUENCY == 0)
            emit htmlLoaded(LOAD_SIGNAL_FREQUENCY);
#endif
    }
    emit localLoadFinished(QThread::currentThread(), titleList);
}


/*--------------------------
* InvertedList::localQuery
*     Receive signals from InvertedList, start searching in the local hashmap,
* and emit a signal of result back.
* Parameter:
*     const QStringList& keyWordList - The key word list.
----------------------------*/
void LocalInvertedList::localQuery(const QStringList& keyWordList)
{
    QList<Html*> resultList;
    for (QString word : keyWordList)
    {
        /* Get the indexList from inverted list */
        List<Index>* indexList = nullptr;
        List<Index>** pIndexList = localHashMap->get(word);
        if (pIndexList == nullptr)
            continue;
        else
            indexList = *pIndexList;

        /* Traverse all index and put them into sorted list */
        for (int i = 0; i < indexList->size(); i++)
        {
            Index* index = &indexList->get(i);
            Html* html = index->getHtml();

            switch (html->getWeightType())
            {
            case Html::WeightType::NotAssigned:
            {
                /* Set html's weight if title contains key words */
                for (QString word : keyWordList)
                {
                    if (html->getTitle().contains(word))
                    {
                        html->setWeightType(Html::WeightType::InTitle);
                        html->setWeight(html->getWeight() + word.size());
                    }
                    html->setWeight(html->getWeight() / html->getTitle().size());
                }
                /* If html's weight type hasn't been set it means it's InContent */
                if (html->getWeightType() == Html::WeightType::NotAssigned)
                    html->setWeightType(Html::WeightType::InContent);

                /* Set brief for html */
                if (html->getBrief().isEmpty())
                {
                    /* Find and pick out a 100 word list that contains most keywords */
                    const QStringList &textContent = html->getText();
                    /* List that determines whether the word is keyword or not */
                    QList<int> textFlags;
                    /* Fills the textFlags list */
                    for (const QString &word : textContent)
                        if (keyWordList.contains(word))
                            textFlags.append(1);
                        else
                            textFlags.append(0);
                    /* Judge and pick out */
                    int maxIndex = 0, maxCount = 0;
                    for (int i = 0; i < textFlags.size() - 100; i++)
                    {
                        int count = 0;
                        for (int j = 0; j < 100; j++)
                            count += textFlags[i + j];

                        if (count > maxCount)
                        {
                            maxCount = count;
                            maxIndex = i;
                        }
                    }
                    /* Set the brief */
                    html->setBrief(textContent.mid(maxIndex, 100).join(QString("")));
                }

                resultList.append(html);
                break;
            }
            case Html::WeightType::InContent:
            {
                html->setWeight(html->getWeight() + index->getFrequency() * word.size());
                break;
            }
            /* Any html whose weight type is Html::WeightType::InTitle is processed at the first time. */
            default:break;
            }
        }
    }
    emit localQueryResult(QThread::currentThread(), resultList);
}
