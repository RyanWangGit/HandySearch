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
	this->hasAnalyzed = false;
	this->file.setFileName(filePath);
	this->load();
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
	return this->textContent;
}


/*--------------------------
* Html::getTitle
* 	Returns the title of the html file.
* Returns:	QString& - Title of html.
----------------------------*/
QString& Html::getTitle()
{
	return this->title;
}


/*--------------------------
* Html::getFilePath
* 	Returns the path of the html file.
* Returns:	QString - Path of html file.
----------------------------*/
QString Html::getFilePath()
{
	return this->file.fileName();
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
	if (!this->textContent.isEmpty())
		this->textContent.clear();

	if (this->file.isOpen())
	{
		//Copy
		this->textContent = fileContent;

		//Remove line breaks and tabs
		this->textContent.replace(QRegExp("[\r|\n|\t]"), "");

		//Remove header
		this->textContent.replace(QRegExp("<head>.*</head>"), "");
		
		//Remove scripts
		this->textContent.replace(QRegExp("<( )*script([^>])*>"), "<script>");
		this->textContent.replace(QRegExp("<script>.*</script>"), "");

		//Remove all styles
		this->textContent.replace(QRegExp("<( )*style([^>])*>"), "<style>");
		this->textContent.replace(QRegExp("<style>.*</style>"), "");
		
		//Remove td tags
		this->textContent.replace(QRegExp("<( )*td([^>])*>"), "");

		//Insert line breaks in <br> and <li> tags
		this->textContent.replace(QRegExp("<( )*br( )*>"), "\n");
		this->textContent.replace(QRegExp("<( )*li( )*>"), "\n");
		
		//Insert line paragraphs in <tr> and <p> tags
		this->textContent.replace(QRegExp("<( )*tr( )*>"), "\r");
		this->textContent.replace(QRegExp("<( )*p( )*>"), "\r");

		//Remove anything that's enclosed inside < >
		this->textContent.replace(QRegExp("<[^>]*>"), "");
		
		//Replace special characters
		this->textContent.replace(QRegExp("&amp;"), "&");
		this->textContent.replace(QRegExp("&nbsp;"), " ");
		this->textContent.replace(QRegExp("&lt;"), "<");
		this->textContent.replace(QRegExp("&gt;"), ">");
		this->textContent.replace(QRegExp("&(.{2,6});"), "");

		//Remove extra line breaks
		this->textContent.replace(QRegExp(" ( )+"), "");
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
	this->title = fileContent;
	QRegExp rx("<title>(.*)</title>");
	rx.setMinimal(true);
	this->title.indexOf(rx);
	
	this->title = rx.cap(1);
}


/*--------------------------
* Html::analyze
* 	Analyze the html and do the word segmentation,create index and 
* put the index into inverted list.
----------------------------*/
void Html::analyze()
{
	if (this->hasAnalyzed)
		return;
	else
		this->hasAnalyzed = true;

	unsigned int pos = 0;
	QString content = this->textContent;
	content.append(" " + this->title);
	WordSegmenter ws(content, HandySearch::dictionary);

	QStringList result = ws.getResult();
	for (QString word : result) 
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
	if (this->file.exists())
		if (!this->file.open(QIODevice::OpenModeFlag::ReadOnly | QIODevice::OpenModeFlag::Text))
			return false;

	if (this->file.isOpen() && this->file.isReadable())
	{
		fileContent = this->file.readAll();
		extractTitle(fileContent);
		extractText(fileContent);
		this->fileName = file.fileName();
		this->file.close();
		/**
		*	The analysis of html file was not called
		*	here because HandySearch::index was not thread-safe
		*	thus making it necessary to call the Html::analyze
		*	in the main load thread instead of calling it in 
		*	sub-thread(by sending signals to the main load thread).
		*/
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
	this->file.close();
	this->file.setFileName(filePath);
	return this->load();
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
	return (this->file.fileName() == other.file.fileName());
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
