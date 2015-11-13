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
#include "Html.h"

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
	analyzed = false;
	weight = 0;
	type = WeightType::NotAssigned;
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
	this->analyzed = c.analyzed;
	this->file.setFileName(c.file.fileName());
	this->textContent = c.textContent;
	this->title = c.title;
}


/*--------------------------
* Html::getText
* 	Returns the pure text of the html file.
* Returns:	QString& - Pure text of html.
----------------------------*/
const QString& Html::getText()
{
	return textContent;
}


/*--------------------------
* Html::getTitle
* 	Returns the title of the html file.
* Returns:	QString& - Title of html.
----------------------------*/
const QString& Html::getTitle()
{
	return title;
}


/*--------------------------
* Html::getFilePath
* 	Returns the path of the html file.
* Returns:	QString - Path of html file.
----------------------------*/
const QString Html::getFilePath()
{
	return file.fileName();
}


const QString& Html::getBrief()
{
	return brief;
}

void Html::setBrief(const QString& brief)
{
	this->brief = brief;
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
	/* If has processed */
	if (!textContent.isEmpty())
		return;

	if (file.isOpen())
	{
		/* Copy */
		textContent = fileContent;

		/* Remove line breaks and tabs */
		textContent.replace(QRegularExpression("[\r|\n|\t]"), "");

		/* Remove notes */
		textContent.replace(QRegularExpression("<!--.[^-]*(?=-->)-->"), "");

		/* Remove header */
		textContent.replace(QRegularExpression("<head>.*</head>"), "");

		/*Remove scripts */
		textContent.replace(QRegularExpression("<( )*script([^>])*>"), "<script>");
		textContent.replace(QRegularExpression("<script>.*</script>"), "");

		/* Remove all styles */
		textContent.replace(QRegularExpression("<( )*style([^>])*>"), "<style>");
		textContent.replace(QRegularExpression("<style>.*</style>"), "");

		/* Remove td tags */
		textContent.replace(QRegularExpression("<( )*td([^>])*>"), "");

		/* Insert line breaks in <br> and <li> tags */
		textContent.replace(QRegularExpression("<( )*br( )*>"), " ");
		textContent.replace(QRegularExpression("<( )*li( )*>"), " ");

		/* Insert line paragraphs in <tr> and <p> tags */
		textContent.replace(QRegularExpression("<( )*tr( )*>"), " ");
		textContent.replace(QRegularExpression("<( )*p( )*>"), " ");

		/* Remove anything that's enclosed inside < > */
		textContent.replace(QRegularExpression("<[^>]*>"), "");

		/* Replace special characters */
		textContent.replace(QRegularExpression("&amp;"), "&");
		textContent.replace(QRegularExpression("&nbsp;"), " ");
		textContent.replace(QRegularExpression("&lt;"), "<");
		textContent.replace(QRegularExpression("&gt;"), ">");
		textContent.replace(QRegularExpression("&(.{2,6});"), "");

		/* Remove line breaks and tabs */
		textContent.replace(QRegularExpression("[ ]+"), " ");
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
	QRegularExpression rx("<title>(.*)</title>", QRegularExpression::InvertedGreedinessOption);
	QRegularExpressionMatch match = rx.match(fileContent);
	title = match.captured(1);
}


bool Html::hasAnalyzed() const
{
	return analyzed;
}


void Html::setAnalyzed(bool analyzed)
{
	this->analyzed = analyzed;
}


Html::WeightType Html::getWeightType() const
{
	return type;
}

void Html::setWeightType(Html::WeightType type)
{
	this->type = type;
}

float Html::getWeight() const
{
	return weight;
}


void Html::setWeight(const float weight)
{
	this->weight = weight;
}


void Html::clearWeight()
{
	brief.clear();
	setWeightType(WeightType::NotAssigned);
	setWeight(0);
}

/*--------------------------
* Html::load
* 	Load the html file only if the path is set.
* Returns:	bool - Result of the load.
----------------------------*/
bool Html::load()
{
	QString fileContent;

	/* Open the file */
	if (file.exists())
		if (!file.open(QIODevice::OpenModeFlag::ReadOnly | QIODevice::OpenModeFlag::Text))
			return false;

	if (file.isOpen() && file.isReadable())
	{
		fileContent = file.readAll();
		extractTitle(fileContent);
		extractText(fileContent);
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
bool Html::loadFrom(const QString &filePath)
{
	file.close();
	file.setFileName(filePath);
	return load();
}


bool Html::operator<(const Html& other)
{
	if (this->getWeightType() < other.getWeightType())
		return true;
	else if (this->getWeightType() == other.getWeightType())
		return this->getWeight() < other.getWeight();
	else
		return false;
}

/*--------------------------
* Html::operator==
* 	Override operator method for two html objects to compare
* (by comparing their file names).
* Returns:	bool - Whether they's equal or not.
* Parameter:
* 	Html & other - The other html object.
----------------------------*/
bool Html::operator== (const Html &other)
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
