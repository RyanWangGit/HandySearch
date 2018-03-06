/*************************************
 * Copyright(C),2015-2016,Ryan Wang 
 * 
 * File:    Html.cpp
 *
 * Version: V1.0
 * 
 * Brief:    This is the implementations of Html class,
 * provides wrapper of html files,providing several operations
 * of the html file.
 *
 * Author:    Ryan
 
 * Date:    Oct. 2015
*************************************/
#include "stable.h"
#include "webpage.h"
#include "word_segmenter.h"
#include "handy_search.h"

/* Initialize the static member */
unsigned int Webpage::totalNum = 0;


Webpage::Webpage() {  }


Webpage::Webpage(const QString &filePath)
{
    totalNum++;
    analyzed = false;
    weight = 0;
    type = WeightType::NotAssigned;
    file.setFileName(filePath);
    load();
}


Webpage::Webpage(const Webpage &c)
{
    this->analyzed = c.analyzed;
    this->file.setFileName(c.file.fileName());
    this->textContent = c.textContent;
    this->title = c.title;
}


const QStringList& Webpage::getText()
{
    return textContent;
}


const QString& Webpage::getTitle()
{
    return title;
}


const QString Webpage::getFilePath()
{
    return file.fileName();
}


const QString& Webpage::getBrief()
{
    return brief;
}


void Webpage::setBrief(const QString& brief)
{
    this->brief = brief;
}


unsigned int Webpage::getTotalHtmlCount()
{
    return Webpage::totalNum;
}


void Webpage::extractText(const QString &fileContent)
{
    /* If has processed */
    if (!textContent.isEmpty())
        return;

    QString text;
    if (file.isOpen())
    {
        /* Copy */
        text = fileContent;

        /* Remove line breaks and tabs */
        text.replace(QRegularExpression("[\r|\n|\t]"), "");

        /* Remove notes */
        text.replace(QRegularExpression("<!--.[^-]*(?=-->)-->"), "");

        /* Remove header */
        text.replace(QRegularExpression("<head>.*</head>"), "");

        /*Remove scripts */
        text.replace(QRegularExpression("<( )*script([^>])*>"), "<script>");
        text.replace(QRegularExpression("<script>.*</script>"), "");

        /* Remove all styles */
        text.replace(QRegularExpression("<( )*style([^>])*>"), "<style>");
        text.replace(QRegularExpression("<style>.*</style>"), "");

        /* Remove td tags */
        text.replace(QRegularExpression("<( )*td([^>])*>"), "");

        /* Insert line breaks in <br> and <li> tags */
        text.replace(QRegularExpression("<( )*br( )*>"), " ");
        text.replace(QRegularExpression("<( )*li( )*>"), " ");

        /* Insert line paragraphs in <tr> and <p> tags */
        text.replace(QRegularExpression("<( )*tr( )*>"), " ");
        text.replace(QRegularExpression("<( )*p( )*>"), " ");

        /* Remove anything that's enclosed inside < > */
        text.replace(QRegularExpression("<[^>]*>"), "");

        /* Replace special characters */
        text.replace(QRegularExpression("&amp;"), "&");
        text.replace(QRegularExpression("&nbsp;"), " ");
        text.replace(QRegularExpression("&lt;"), "<");
        text.replace(QRegularExpression("&gt;"), ">");
        text.replace(QRegularExpression("&(.{2,6});"), "");

        /* Remove line breaks and tabs */
        text.replace(QRegularExpression("[ ]+"), " ");
    }
    /* Do word segmentation and set the member */
    text.append(" " + title);

    WordSegmenter ws(HandySearch::getInstance()->getDictionary());
    textContent = ws.segment(text);
}


void Webpage::extractTitle(const QString &fileContent)
{
    QRegularExpression rx("<title>(.*)</title>", QRegularExpression::InvertedGreedinessOption);
    QRegularExpressionMatch match = rx.match(fileContent);
    title = match.captured(1);
}


bool Webpage::hasAnalyzed() const
{
    return analyzed;
}


void Webpage::setAnalyzed(bool analyzed)
{
    this->analyzed = analyzed;
}


Webpage::WeightType Webpage::getWeightType() const
{
    return type;
}


void Webpage::setWeightType(Webpage::WeightType type)
{
    this->type = type;
}


float Webpage::getWeight() const
{
    return weight;
}


void Webpage::setWeight(const float weight)
{
    this->weight = weight;
}


void Webpage::clearWeight()
{
    brief.clear();
    setWeightType(WeightType::NotAssigned);
    setWeight(0);
}


bool Webpage::load()
{
    QString fileContent;

    /* Open the file */
    if (file.exists())
        if (!file.open(QIODevice::OpenModeFlag::ReadOnly | QIODevice::OpenModeFlag::Text))
            return false;

    if (file.isOpen() && file.isReadable())
    {
        fileContent = file.readAll();
        extractTitle(fileContent);
        extractText(fileContent);
        fileName = file.fileName();
        file.close();

        return true;
    }
    else
        return false;
}


bool Webpage::loadFrom(const QString &filePath)
{
    file.close();
    file.setFileName(filePath);
    return load();
}


bool Webpage::operator<(const Webpage& other)
{
    if (this->getWeightType() < other.getWeightType())
        return true;
    else if (this->getWeightType() == other.getWeightType())
        return this->getWeight() < other.getWeight();
    else
        return false;
}


bool Webpage::operator== (const Webpage &other)
{
    return (file.fileName() == other.file.fileName());
}


Webpage& Webpage::operator= (const Webpage &other)
{
    if (this == &other)
        return *this;

    this->file.setFileName(other.file.fileName());
    this->textContent = other.textContent;
    this->title = other.title;
    return *this;
}
