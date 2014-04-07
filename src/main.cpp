#include "mainwindow.h"
#include <QApplication>
#include <QSettings>
#include "global.h"
int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QCoreApplication::setOrganizationName("rnae.de");
  QCoreApplication::setOrganizationDomain("www.rnae.de");
  QCoreApplication::setApplicationName("Ardulight");

  Global& g = Global::getInstance();

  Global::getInstance().getSettings().setValue("Main/test", "ASDASDADSD");
}
