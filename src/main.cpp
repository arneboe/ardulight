#include "mainwindow.h"
#include <QApplication>
#include <QSettings>
#include "global.h"
#include "light.h"
#include <QSystemTrayIcon>
#include "traymenu.h"
int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QCoreApplication::setOrganizationName("rnae.de");
  QCoreApplication::setOrganizationDomain("www.rnae.de");
  QCoreApplication::setApplicationName("Ardulight");

  Light l;
  for(short i = 0; i < 60; ++i)
  {
    l.setColor(i,255,255,255);
  }
  l.show();

  QSystemTrayIcon tray(QIcon(":/resources/lightbulb-1.ico"));
  TrayMenu* menu = new TrayMenu();
  tray.setContextMenu(menu);//menu should be deleted when tray is destroyed
  tray.show();
  a.exec();
}
