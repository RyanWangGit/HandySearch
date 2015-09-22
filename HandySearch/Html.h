#pragma once

/* This class first tries to open the file the path indicates, 
and uses some methods to return different kinds of data */
class Html 
{
private:
	/* The html file */
	QFile file;
	/* The file name */
	QString fileName;
	/* Pure text content extracted from file content */
	QString textContent;
	/* Title extracted from file content*/
	QString title;
	bool hasAnalyzed;
	void extractText(const QString &fileContent);
	void extractTitle(const QString &fileContent);
	bool load();
public:
	static unsigned int totalNum;
	Html();
	Html(const QString &filePath);
	Html(const Html &c);
	QString &getText();
	QString &getTitle();
	QString getFilePath();
	bool loadFrom(QString &filePath);
	bool operator== (Html &other);
	Html& operator= (const Html &other);
	int hashCode();
	/* Segment the content and save it to the inverted list */
	void analyze();
};

