#include "mainwindow.h"
#include <QApplication>
#include <QSettings>
#include "global.h"
#include "light.h"
#include <QSystemTrayIcon>
#include <QThread>
#include "traymenu.h"
#include "Screen.h"
#include <QLabel>
#include <QDateTime>
#include "ambilightmode.h"
int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QCoreApplication::setOrganizationName("rnae.de");
  QCoreApplication::setOrganizationDomain("www.rnae.de");
  QCoreApplication::setApplicationName("Ardulight");


//  Screen x;
//  x.initialize(0);
//  Screen::Pixel* pData = new Screen::Pixel[x.getHeight() * x.getWidth()];
//  x.getScreenBitmap(pData);

//  QImage img(x.getWidth(), x.getHeight(),QImage::Format_ARGB32);
//  for(int h = 0; h < x.getHeight(); ++h)
//  {
//    for(int w = 0; w < x.getWidth(); ++w)
//    {
//      img.setPixel(w, h, );
//    }
//  }
//  QLabel myLabel;
//  myLabel.setPixmap(QPixmap::fromImage(img));
//  myLabel.show();


  QSystemTrayIcon tray(QIcon(":/resources/lightbulb-1.ico"));
  TrayMenu* menu = new TrayMenu();
  tray.setContextMenu(menu);//menu should be deleted when tray is destroyed
  tray.show();
  AmbilightMode mode;
  mode.startMode();
  a.exec();
}
