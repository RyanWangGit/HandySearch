#include "stdafx.h"
#include "LocalInvertedList.h"
#include "WordSegmenter.h"
#include "HandySearch.h"


/*--------------------------
* LocalInvertedList::putToInvertedList
* 	Analyze the html and do the word segmentation,create index and
* put the index into inverted list.
*
* Parameter:
* 	Html * html - The html pointer which is to be operated.
----------------------------*/
void LocalInvertedList::putInLocalList(Html* html)
{
	if (html->hasAnalyzed())
		return;
	else
		html->setAnalyzed(true);

	unsigned int pos = 0;
	QString content = html->getText();
	content.append(" " + html->getTitle());
	WordSegmenter ws(content, HandySearch::getInstance()->getDictionary());

	QStringList wordList = ws.getResult();

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
			indexList->append(Index(temp, pos));
		}
	}
}

/*--------------------------
* LocalInvertedList::LocalInvertedList
* 	Local inverted list constructor.
* Parameter:
* 	const QStringList & pathList - Path of files.
----------------------------*/
LocalInvertedList::LocalInvertedList(const QStringList& pathList)
{
	this->localHashMap = new HashMap<List<Index>*>();
	this->pathList = pathList;
}


LocalInvertedList::~LocalInvertedList()
{
	/* Delete the content of hashmap */
	for (List<Index>* indexList : hashMapContent)
		delete indexList;

	delete localHashMap;
}

/*--------------------------
* LocalInvertedList::load
* 	Load several html files,this is a sub-task,managed by HtmlLoadTask.
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
					html->setBrief(html->getText().mid(index->getPosition().get(0) - HandySearch::getInstance()->getDictionary()->getMaxLength(), 200));

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
