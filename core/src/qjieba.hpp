#pragma once
#include <QString>
#include <QStringList>
#include <cppjieba/Jieba.hpp>


// A QT adapter for cppjieba
class QJieba {
public:
  QJieba(const QString &dictPath, const QString &modelPath,
         const QString &userPath, const QString &idfPath,
         const QString &stopWords)
    : jieba(new cppjieba::Jieba(dictPath.toStdString(),
                                modelPath.toStdString(),
                                userPath.toStdString(),
                                idfPath.toStdString(),
                                stopWords.toStdString()))
  {

  }

  QStringList cut(const QString &sentence) {
    QStringList words;
    std::vector<std::string> stdWords;
    this->jieba->Cut(sentence.toStdString(), stdWords);
    for(const std::string &word: stdWords)
    {
      words.append(QString::fromStdString(word));
    }
    return words;
  }

private:
  std::unique_ptr<cppjieba::Jieba> jieba;
};
