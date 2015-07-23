#include "stdafx.h"
#include "Html.h"

/* Initialization of static members */
unsigned int Html::totalNum = 0;

Html::Html() { void; }

Html::Html(const QString &filePath)
{
	this->file.setFileName(filePath);
	this->load();
}

Html::Html(const Html &c)
{
	this->file.setFileName(c.file.fileName());
	this->textContent = c.textContent;
	this->title = c.title;
}

unsigned int Html::getID()
{
	return this->htmlID;
}

QString& Html::getText()
{
	return this->textContent;
}
QString& Html::getTitle()
{
	return this->title;
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
		this->textContent.replace("\r", "");
		this->textContent.replace("\n", "");
		this->textContent.replace("\t", "");

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
		
		this->file.close();
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
	return HashMap<int, int>::hashCode(str.data(), str.size());
}