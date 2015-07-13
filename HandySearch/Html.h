#pragma once

/* This class first tries to open the file the path indicates, 
and uses some methods to return different kinds of data */
class Html
{
private:
	QString filePath;
	QFile* file;
	/* Whole pure text content extracted from file content*/
	QString textContent;
	/* Title extracted from file content*/
	QString title;
	void extractText(QString& fileContent);
	void extractTitle(QString& fileContent);

public:
	Html(QString filePath);
	~Html();
	bool isOpen();
	QString& getText();
	QString& getTitle();
};

