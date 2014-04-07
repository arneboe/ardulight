#include "mainwindow.h"
#include <QApplication>
#include <QSettings>
#include "global.h"
#include "light.h"
#include <QSystemTrayIcon>
int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QCoreApplication::setOrganizationName("rnae.de");
  QCoreApplication::setOrganizationDomain("www.rnae.de");
  QCoreApplication::setApplicationName("Ardulight");

  Light l;
  for(short i = 0; i < 20; ++i)
  {
    l.setColor(i,0,0,255);
  }
  l.show();

  QSystemTrayIcon tray(QIcon(":/resources/lightbulb-1.ico"));
  tray.show();
  a.exec();

}
