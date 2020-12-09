#include "handy_search.h"
#include "searchcore.h"


int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  qRegisterMetaType<QList<Webpage> >("QList<Webpage>");

  HandySearch w;
  /* Start loading and check the directory */
  if (!w.load())
    return 1;

  return a.exec();
}
