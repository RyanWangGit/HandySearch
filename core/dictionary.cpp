#include "stable.h"
#include "dictionary.h"


/*--------------------------
* Dictionary::Dictionary
*     This is the common-used constructor.
----------------------------*/
Dictionary::Dictionary()
{
    this->maxLength = 0;
}

/*--------------------------
* Dictionary::hasItem
*     Interface to BloomFilter, provides support for QString.
* Returns:    bool - Result.
* Parameter:
*    const QString & key - The key to search.
----------------------------*/
bool Dictionary::hasItem(const QString &key) const
{
    if (key == "")
        return false;
    QByteArray ba = key.toLocal8Bit();
    char * str = ba.data();
    return bf.hasItem(str, ba.size());
}


/*--------------------------
* Dictionary::addItem
*     Interface to BloomFilter, provides support for QString.
* Returns:    bool - Operation result.
* Parameter:
*     const QString & key - The key to add.
----------------------------*/
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


/*--------------------------
* Dictionary::getMaxLength
*     Get the maximum word length of the dictioanry.
* Returns:    unsigned int - Maximum word length.
----------------------------*/
unsigned int Dictionary::getMaxLength() const
{
    return maxLength;
}

/*--------------------------
* Dictionary::load
*     Start loading dictionary.
----------------------------*/
void Dictionary::load()
{
    /* If dictionary folder doesn't exist */
    if (!dictFolder.exists())
        return;

    emit dictLoadStarted();
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
            if (i % 1000 == 0)
                emit dictLoaded(1000);
        }
        file.close();
    }
    emit dictLoadFinished();
}


/*--------------------------
* Dictionary::setDictFolder
*     Set the dictionary loading folder, loading process won't work
* if the directory is not correctly set.
* Parameter:
*     const QDir & dictFolder - The folder to set.
----------------------------*/
void Dictionary::setDictFolder(const QDir& dictFolder)
{
    this->dictFolder = dictFolder;
}
