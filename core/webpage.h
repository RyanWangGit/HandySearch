#pragma once

#include <QString>

class Webpage
{
private:
    QString title;
    QString content;
    QString url;
public:
    Webpage(const QString &title, const QString &content, const QString &url);
    inline const QString &getTitle() { return this->title; }
    inline const QString &getContent() { return this->content; }
    inline const QString &getUrl() { return this->url; }
};
