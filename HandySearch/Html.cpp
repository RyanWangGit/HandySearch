#include "stdafx.h"
#include "Html.h"

/* Initialize the static member */
unsigned int Html::totalNum = 0;

Html::Html() { void; }

Html::Html(const QString &filePath)
{
	this->hasAnalyzed = false;
	this->file.setFileName(filePath);
	this->load();
}

Html::Html(const Html &c)
{
	this->hasAnalyzed = c.hasAnalyzed;
	this->file.setFileName(c.file.fileName());
	this->textContent = c.textContent;
	this->title = c.title;
}

QString& Html::getText()
{
	return this->textContent;
}
QString& Html::getTitle()
{
	return this->title;
}

QString Html::getFilePath()
{
	return this->file.fileName();
}

/* Extract pure text from html file and store it into Html::textContent */
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

/* Extract title tag from html file and store it into Html::title */
void Html::extractTitle(const QString &fileContent)
{
	this->title = fileContent;
	QRegExp rx("<title>(.*)</title>");
	rx.setMinimal(true);
	this->title.indexOf(rx);
	
	this->title = rx.cap(1);
}

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

		bool hasFound = false;
		for (int i = 0; i < indexList->size(); i++)
		{
			Index* index = &indexList->get(i);
			//If the word belongs to an existed index
			if (index->getHtml() == this)
			{
				index->getPosition().append(pos);
				hasFound = true;
				break;
			}
		}
		if (!hasFound)
		{
			Html* temp = this;
			indexList->append(Index(temp, pos));
		}
	}
}

/* Load the html file only if the path is set */
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
		//this->analyze();
		return true;
	}
	else
		return false;
}

/* Reload from file */
bool Html::loadFrom(QString &filePath)
{
	this->file.close();
	this->file.setFileName(filePath);
	return this->load();
}

/* Html objects compare */
bool Html::operator== (Html &other)
{
	return (this->file.fileName() == other.file.fileName());
}

Html& Html::operator= (const Html &other)
{
	if (this == &other)
		return *this;

	this->file.setFileName(other.file.fileName());
	this->textContent = other.textContent;
	this->title = other.title;
	return *this;
}

int Html::hashCode()
{
	QByteArray str = this->title.toLocal8Bit();
	return HashMap<int>::hashCode(str.data(), str.size());
}