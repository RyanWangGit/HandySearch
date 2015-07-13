#include "stdafx.h"
#include "Html.h"


Html::Html(QString filePath)
{
	QString fileContent;

	this->file = new QFile(filePath);

	if (file->exists())
		this->file->open(QIODevice::OpenModeFlag::ReadOnly | QIODevice::OpenModeFlag::Text);

	if (this->isOpen() && this->file->isReadable())
	{
		//File reading stream
		QTextStream in(this->file);
		in.setCodec("utf-8");
		fileContent = in.readAll();

		extractTitle(fileContent);
		extractText(fileContent);	
	}
}


Html::~Html()
{
	this->file->close();
	delete this->file;
}

bool Html::isOpen()
{
	return (this->file->isOpen() && this->file->isReadable());
}

QString& Html::getText()
{
	return this->textContent;
}
QString& Html::getTitle()
{
	return this->title;
}

//Extract pure text from html file 
//and store it into Html::textContent
void Html::extractText(QString& fileContent)
{
	//If has processed
	if (!this->textContent.isEmpty())
		return;

	if (this->file->isOpen())
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

//Extract title tag from html file 
//and store it into Html::title
void Html::extractTitle(QString& fileContent)
{
	this->title = fileContent;
	QRegExp rx("<title>(.*)</title>");
	this->title.indexOf(rx);
	
	this->title = rx.cap(1);
}