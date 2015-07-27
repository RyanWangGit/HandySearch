#pragma once

/* This class first tries to open the file the path indicates, 
and uses some methods to return different kinds of data */
class Html 
{
private:
	static unsigned int totalNum;
	unsigned int htmlID;
	/* The html file */
	QFile file;
	/* Pure text content extracted from file content */
	QString textContent;
	/* Title extracted from file content*/
	QString title;
	void extractText(const QString &fileContent);
	void extractTitle(const QString &fileContent);
	bool load();
public:
	Html();
	Html(const QString &filePath);
	Html(const Html &c);
	unsigned int getID();
	QString &getText();
	QString &getTitle();
	QString &getFilePath();
	bool loadFrom(QString &filePath);
	bool operator== (Html &other);
	Html& operator= (const Html &other);
	int hashCode();
};

