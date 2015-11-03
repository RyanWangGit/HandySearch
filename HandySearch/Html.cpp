/*************************************
 * Copyright(C),2015-2016,Ryan Wang 
 * 
 * File:	Html.cpp
 *
 * Version: V1.0
 * 
 * Brief:	This is the implementations of Html class,
 * provides wrapper of html files,providing several operations
 * of the html file.
 *
 * Author:	Ryan
 
 * Date:	Oct. 2015
*************************************/
#include "stdafx.h"
#include "List.h"
#include "Html.h"
#include "Index.h"
#include "WordSegmenter.h"
#include "HandySearch.h"

/* Initialize the static member */
unsigned int Html::totalNum = 0;


/*--------------------------
* Html::Html
* 	Default void constructor,created for list's head node.
----------------------------*/
Html::Html() { void; }


/*--------------------------
* Html::Html
* 	Load from the file path,this is the common-used constructor.
* Parameter:
* 	const QString & filePath - Path of html file.
----------------------------*/
Html::Html(const QString &filePath)
{
	totalNum++;
	hasAnalyzed = false;
	file.setFileName(filePath);
	load();
}


/*--------------------------
* Html::Html
* 	Copy constructor.
* Parameter:
* 	const Html & c - The other html object.
----------------------------*/
Html::Html(const Html &c)
{
	this->hasAnalyzed = c.hasAnalyzed;
	this->file.setFileName(c.file.fileName());
	this->textContent = c.textContent;
	this->title = c.title;
}


/*--------------------------
* Html::getText
* 	Returns the pure text of the html file.
* Returns:	QString& - Pure text of html.
----------------------------*/
QString& Html::getText()
{
	return textContent;
}


/*--------------------------
* Html::getTitle
* 	Returns the title of the html file.
* Returns:	QString& - Title of html.
----------------------------*/
QString& Html::getTitle()
{
	return title;
}


/*--------------------------
* Html::getFilePath
* 	Returns the path of the html file.
* Returns:	QString - Path of html file.
----------------------------*/
QString Html::getFilePath()
{
	return file.fileName();
}


unsigned int Html::getTotalHtmlCount()
{
	return Html::totalNum;
}

/*--------------------------
* Html::extractText
* 	Extract pure text from html file and store it into Html::textContent.
* Parameter:
* 	const QString & fileContent - Content of html file(with all html labels).
----------------------------*/
void Html::extractText(const QString &fileContent)
{
	//If has processed
	if (!textContent.isEmpty())
		textContent.clear();

	if (file.isOpen())
	{
		//Copy
		textContent = fileContent;

		//Remove line breaks and tabs
		textContent.replace(QRegExp("[\r|\n|\t]"), "");

		//Remove header
		textContent.replace(QRegExp("<head>.*</head>"), "");
		
		//Remove scripts
		textContent.replace(QRegExp("<( )*script([^>])*>"), "<script>");
		textContent.replace(QRegExp("<script>.*</script>"), "");

		//Remove all styles
		textContent.replace(QRegExp("<( )*style([^>])*>"), "<style>");
		textContent.replace(QRegExp("<style>.*</style>"), "");
		
		//Remove td tags
		textContent.replace(QRegExp("<( )*td([^>])*>"), "");

		//Insert line breaks in <br> and <li> tags
		textContent.replace(QRegExp("<( )*br( )*>"), "\n");
		textContent.replace(QRegExp("<( )*li( )*>"), "\n");
		
		//Insert line paragraphs in <tr> and <p> tags
		textContent.replace(QRegExp("<( )*tr( )*>"), "\r");
		textContent.replace(QRegExp("<( )*p( )*>"), "\r");

		//Remove anything that's enclosed inside < >
		textContent.replace(QRegExp("<[^>]*>"), "");
		
		//Replace special characters
		textContent.replace(QRegExp("&amp;"), "&");
		textContent.replace(QRegExp("&nbsp;"), " ");
		textContent.replace(QRegExp("&lt;"), "<");
		textContent.replace(QRegExp("&gt;"), ">");
		textContent.replace(QRegExp("&(.{2,6});"), "");

		//Remove extra line breaks
		textContent.replace(QRegExp(" ( )+"), "");
	}
}


/*--------------------------
* Html::extractTitle
* 	Extract title tag from html file and store it into Html::title.
* Parameter:
* 	const QString & fileContent - Content of html file(with all html labels).
----------------------------*/
void Html::extractTitle(const QString &fileContent)
{
	title = fileContent;
	QRegExp rx("<title>(.*)</title>");
	rx.setMinimal(true);
	title.indexOf(rx);
	
	title = rx.cap(1);
}


/*--------------------------
* Html::analyze
* 	Analyze the html and do the word segmentation,create index and 
* put the index into inverted list.
----------------------------*/
void Html::analyze()
{
	if (hasAnalyzed)
		return;
	else
		hasAnalyzed = true;

	unsigned int pos = 0;
	QString content = textContent;
	content.append(" " + title);
	WordSegmenter ws(content, HandySearch::dictionary);

	QStringList wordList = ws.getResult();

	for (QString word : wordList)
	{
		pos += word.size();
		//If the first character isn't chinese
		QChar ch = word.at(0);	
		if (!(ch.unicode() >= 0x4e00 && ch.unicode() <= 0x9FA5))
			continue;
		
		//Find if there's a list existed
		List<Index>* indexList = nullptr;
		List<Index>** pIndexList = HandySearch::index.get(word);
		if (pIndexList == nullptr)
		{
			indexList = new List<Index>();
			HandySearch::index.put(word, indexList);
		}
		else
			indexList = *pIndexList;

		//Find if the word belongs to an existed index
		bool hasFound = false;
		for (int i = 0; i < indexList->size(); i++)
		{
			Index* index = &indexList->get(i);
			//Yes
			if (index->getHtml() == this)
			{
				index->getPosition().append(pos);
				hasFound = true;
				break;
			}
		}
		//No
		if (!hasFound)
		{
			Html* temp = this;
			indexList->append(Index(temp, pos));
		}
	}
}


/*--------------------------
* Html::load
* 	Load the html file only if the path is set.
* Returns:	bool - Result of the load.
----------------------------*/
bool Html::load()
{
	QString fileContent;

	//Open the file
	if (file.exists())
		if (!file.open(QIODevice::OpenModeFlag::ReadOnly | QIODevice::OpenModeFlag::Text))
			return false;

	if (file.isOpen() && file.isReadable())
	{
		fileContent = file.readAll();
		extractTitle(fileContent);
		extractText(fileContent);
		analyze();
		fileName = file.fileName();
		file.close();

		return true;
	}
	else
		return false;
}


/*--------------------------
* Html::loadFrom
* 	Reload from file.
* Returns:	bool - Result of the load.
* Parameter:
* 	QString & filePath - Path of html file.
----------------------------*/
bool Html::loadFrom(QString &filePath)
{
	file.close();
	file.setFileName(filePath);
	return load();
}


/*--------------------------
* Html::operator==
* 	Override operator method for two html objects to compare
* (by comparing their file names).
* Returns:	bool - Whether they's equal or not.
* Parameter:
* 	Html & other - The other html object.
----------------------------*/
bool Html::operator== (Html &other)
{
	return (file.fileName() == other.file.fileName());
}


/*--------------------------
* Html::operator=
* 	Override operator method to directly assign value,
* avoiding to load twice.
* Returns:	Html& - The html object itself.
* Parameter:
* 	const Html & other - The other html object.
----------------------------*/
Html& Html::operator= (const Html &other)
{
	if (this == &other)
		return *this;

	this->file.setFileName(other.file.fileName());
	this->textContent = other.textContent;
	this->title = other.title;
	return *this;
}
