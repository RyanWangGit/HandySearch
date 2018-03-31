#include "stable.h"
#include "dictionary.h"


Dictionary::Dictionary()
{
    this->maxLength = 0;
}


bool Dictionary::hasItem(const QString &key) const
{
    if (key == "")
        return false;
    QByteArray ba = key.toLocal8Bit();
    char * str = ba.data();
    return bf.hasItem(str, ba.size());
}


bool Dictionary::addItem(const QString &key)
{
    if (key == "")
        return false;
    QByteArray ba = key.toLocal8Bit();
    unsigned int len = ba.size();
    char* str = ba.data();

    /* Update maxWordLength */
    if (len / 2 > maxLength)
        maxLength = len / 2;

    return bf.addItem(str, len);
}


unsigned int Dictionary::getMaxLength() const
{
    return maxLength;
}

{
    /* If dictionary folder doesn't exist */
    if (!dictFolder.exists())
        return;

    /* List all files */
    QStringList pathList = dictFolder.entryList(QDir::NoDotAndDotDot | QDir::Files);
    for (QString& path : pathList)
        path.prepend(dictFolder.absolutePath() + "/");

    /* Traverse dictionary folder */
    for (QString path : pathList)
    {
        /* Ignore those aren't text files*/
        if (!path.endsWith(".txt"))
            continue;

        /* Open dictionary file */
        QFile file(path);
        if (!file.open(QIODevice::OpenModeFlag::ReadOnly | QIODevice::OpenModeFlag::Text))
            continue;

        /* Read dictionary file */
        for (int i = 0; !file.atEnd(); i++)
        {
            QString entry = file.readLine();
            /* Cut out the last '\n' character */
            entry.chop(1);
#ifndef SKIPLOAD
            addItem(entry);
#endif
        }
        file.close();
    }
}

void Dictionary::setDictFolder(const QDir& dictFolder)
{
    this->dictFolder = dictFolder;
}
