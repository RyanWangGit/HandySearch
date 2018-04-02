#include "stable.h"
#include "load_ui.h"
#include "handy_search.h"

/* Initialization of static members */
LoadUI* LoadUI::instance = nullptr;

LoadUI::LoadUI()
{
    ui.setupUi(this);
    /* Initialize variables */
    isPressed = false;
    currentProgress = 0;
    maximumProgress = 0;
    instance = &(*this);

    /* Bind the signal */
    connect(ui.close, &QPushButton::clicked, []{ QApplication::quit(); });

    setWindowIconText("Handy Search");
    setWindowFlags(Qt::FramelessWindowHint);
    setModal(false);
}


bool LoadUI::checkDirectory()
{
    QString currentPath = QApplication::applicationDirPath();
    /* The default html library path */
    htmlFolder = currentPath + "/Html Library";

    /* The default dictionary library path */
    dictFolder = currentPath + "/Dictionary Library";

    /* If html folder or dictionary folder doesn't exist */
    if (!htmlFolder.exists() || !dictFolder.exists())
    {
        while (!dictFolder.exists())
        {
            dictFolder = QFileDialog::getExistingDirectory(this, "Choose Dictionary Library", "");
            if (!dictFolder.exists())
            {
                QApplication::beep();
                if (QMessageBox::question(nullptr, "Warning", "Are you sure you want to quit the application?") == QMessageBox::Yes)
                    return false;
            }
        }
        while (!htmlFolder.exists())
        {
            htmlFolder = QFileDialog::getExistingDirectory(this, "Choose Html Library", "");
            if (!htmlFolder.exists())
            {
                QApplication::beep();
                if (QMessageBox::question(nullptr, "Warning", "Are you sure you want to quit the application?") == QMessageBox::Yes)
                    return false;
            }
        }
    }
    return true;
}


bool LoadUI::loadData()
{
    /* Show up the dialog */
    show();
    /* Check the directory is correct or not */
    if (!checkDirectory())
        return false;

    /* Start the loading clock */
    clock.start();
    
    /* Set dictionary folder and connect UI signals */
    /*
    Dictionary* dict = HandySearch::getInstance()->getDictionary();
    dict->setDictFolder(dictFolder);
    connect(dict, &Dictionary::dictLoadStarted, this, &LoadUI::dictLoadStarted);
    connect(dict, &Dictionary::dictLoaded, this, &LoadUI::dictLoaded);
    connect(dict, &Dictionary::dictLoadFinished, this, &LoadUI::dictLoadFinished);
    */
    /* Set html folder and connect UI signals */
    InvertedList* invertedList = HandySearch::getInstance()->getInvertedList();
    invertedList->setHtmlFolder(htmlFolder);
    connect(invertedList, &InvertedList::htmlLoadStarted, this, &LoadUI::htmlLoadStarted);
    /* htmlLoaded signal is directly connected to LoadUI inside HtmlLoadTask */
    connect(invertedList, &InvertedList::htmlLoadFinished, this, &LoadUI::htmlLoadFinished);
    
    
    /* Connect the loading procedure signals/slots */
    connect(this, &LoadUI::start, this, &LoadUI::loadStarted);
    //connect(this, &LoadUI::start, dict, &Dictionary::load);
    //connect(dict, &Dictionary::dictLoadFinished, invertedList, &InvertedList::load);
    connect(invertedList, &InvertedList::htmlLoadFinished, this, &LoadUI::loadFinished);

    /* Start loading */
    emit start();

    return true;
}


void LoadUI::loadingDots()
{
    //TODO:
    //The implementaion seems stupid and needs to be improved.
    //To be an QWidget
    static int time1 = 0;
    static int time2 = -2;
    static int time3 = -4;
    static int time4 = -6;
    static int time5 = -8;
    QPoint point1 = ui.dot1->pos();
    QPoint point2 = ui.dot2->pos();
    QPoint point3 = ui.dot3->pos();
    QPoint point4 = ui.dot4->pos();
    QPoint point5 = ui.dot5->pos();

    point1.rx() = pow(time1 - 25, 3) * 0.0069 + 200;
    if (point1.rx() >= 350)
        point1.rx() = 50;
    point2.rx() = pow(time2 - 25, 3) * 0.0069 + 194;
    if (point2.rx() >= 344)
        point2.rx() = 44;
    point3.rx() = pow(time3 - 25, 3) * 0.0069 + 188;
    if (point3.rx() >= 338)
        point3.rx() = 38;
    point4.rx() = pow(time4 - 25, 3) * 0.0069 + 182;
    if (point4.rx() >= 332)
        point4.rx() = 32;
    point5.rx() = pow(time5 - 25, 3) * 0.0069 + 176;
    if (point5.rx() >= 326)
        point5.rx() = 26;
    ui.dot1->move(point1);
    ui.dot2->move(point2);
    ui.dot3->move(point3);
    ui.dot4->move(point4);
    ui.dot5->move(point5);

    time1++;
    time2++;
    time3++;
    time4++;
    time5++;
    time1 %= 50;
    time2 %= 50;
    time3 %= 50;
    time4 %= 50;
    time5 %= 50;
}


void LoadUI::mousePressEvent(QMouseEvent *event)
{
    isPressed = true;
    origin = event->pos();
}


void LoadUI::mouseMoveEvent(QMouseEvent *event)
{
    if (isPressed)
        move(event->globalX() - origin.x(), event->globalY() - origin.y());
}


void LoadUI::mouseReleaseEvent(QMouseEvent * event)
{
    isPressed = false;
}


LoadUI::~LoadUI()
{
}

LoadUI* LoadUI::getInstance()
{
    return instance;
}

