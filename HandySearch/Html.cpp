#include "stdafx.h"
#include "Html.h"

Html::Html()
{
	
}

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
		//File reading stream
		QTextStream in(&this->file);
		in.setCodec("utf-8");
		fileContent = in.readAll();
		extractTitle(fileContent);
		extractText(fileContent);
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

	Html *temp = new Html();
	temp->file.setFileName(other.file.fileName());
	temp->textContent = other.textContent;
	temp->title = other.title;
	return *temp;
}

int Html::hashCode()
{
	return HashMap<int, int>::hashCode((void *) &this->title, this->title.size());
}