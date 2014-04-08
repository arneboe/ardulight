#include "mainwindow.h"
#include <QApplication>
#include <QSettings>
#include "global.h"
#include "light.h"
#include <QSystemTrayIcon>
#include <QThread>
#include "traymenu.h"
int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QCoreApplication::setOrganizationName("rnae.de");
  QCoreApplication::setOrganizationDomain("www.rnae.de");
  QCoreApplication::setApplicationName("Ardulight");

  Light l;
  QThread::sleep(5);

  l.setColor(0,255,0,0);
  l.setColor(2,255,0,0);
  l.setColor(4,255,0,0);
  l.setColor(6,255,0,0);
  l.setColor(8,255,0,0);
  l.setColor(10,255,0,0);
  l.setColor(11,255,0,0);
  l.setColor(12,255,0,0);
  l.setColor(13,255,0,0);
  l.setColor(14,255,0,0);
  l.setColor(15,255,0,0);
  l.setColor(16,255,0,0);
  l.setColor(17,255,0,0);
  l.setColor(18,255,0,0);
  l.setColor(19,255,0,0);
  l.sendColors();

//  while(true)
//  {
//    unsigned char r = 0;
//    unsigned char g = 0;
//    unsigned char b = 64;
//    for(int i = 0; i < 60; ++i)
//    {
//      l.setColor(i,r,g,b);
//    }
//    l.show();
//    ++r; ++g; ++b;
//  }



  QSystemTrayIcon tray(QIcon(":/resources/lightbulb-1.ico"));
  TrayMenu* menu = new TrayMenu();
  tray.setContextMenu(menu);//menu should be deleted when tray is destroyed
  tray.show();
  a.exec();
}
