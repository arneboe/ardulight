#include <QApplication>
#include <QSettings>
#include "Global.h"
#include "Light.h"
#include <QSystemTrayIcon>
#include "TrayMenu.h"
#include <memory>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  a.setQuitOnLastWindowClosed(false); //without this opening and closing a dialog will end the application...
  QCoreApplication::setOrganizationName("rnae.de");
  QCoreApplication::setOrganizationDomain("www.rnae.de");
  QCoreApplication::setApplicationName("Ardulight");

  std::shared_ptr<Light> light(new Light());
  QSystemTrayIcon tray(QIcon(":/resources/lightbulb-1.ico"));
  TrayMenu* menu = new TrayMenu(light);
  tray.setContextMenu(menu);//menu should be deleted when tray is destroyed
  tray.show();
  a.exec();
}
