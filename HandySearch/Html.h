/*****************************************
* DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER.
*
* Copyright 2015 Ryan Wang, wangyuxin@hust.edu.cn
*
* This file is part of HandySearch.
* HandySearch is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* HandySearch is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with HandySearch.  If not, see <http://www.gnu.org/licenses/>.
*
* - Author: Ryan Wang
* - Contact: wangyuxin@hust.edu.cn
* - License: GNU Lesser General Public License (LGPL)
* - Blog and source code availability: http://ryanwanggit.github.io/HandySearch/
*****************************************/
#pragma once

/**
 * Class:	Html
 *
 * Brief:	This class first tries to open the file the path indicates,
 * and uses some methods to return different kinds of data.
 *
 * Date:	Oct. 2015
 */
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
	static unsigned int totalNum;
	bool hasAnalyzed;
	void extractText(const QString &fileContent);
	void extractTitle(const QString &fileContent);
	/* Segment the content and save it to the inverted list */
	void analyze();
	bool load();
public:
	Html();
	Html(const QString &filePath);
	Html(const Html &c);
	QString &getText();
	QString &getTitle();
	QString getFilePath();
	static unsigned int getTotalHtmlCount();
	bool loadFrom(QString &filePath);
	bool operator== (Html &other);
	Html& operator= (const Html &other);
};

