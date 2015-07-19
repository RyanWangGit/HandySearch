#pragma once

/* This class first tries to open the file the path indicates, 
and uses some methods to return different kinds of data */
class Html
{
private:
	QFile file;
	/* Pure text content extracted from file content */
	QString textContent;
	/* Title extracted from file content*/
	QString title;
	void extractText(const QString& fileContent);
	void extractTitle(const QString& fileContent);
	bool load();
public:
	Html::Html();
	Html(const QString &filePath);
	Html(const Html &c);
	QString& getText();
	QString& getTitle();
	bool loadFrom(QString& filePath);
	bool operator ==(Html &other);
	int hashCode();
};

