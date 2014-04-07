#include "traymenu.h"
#include <QLabel>
#include <QAction>
#include <QApplication>
TrayMenu::TrayMenu(QWidget *parent) :
  QMenu(parent)
{
  QAction* quitAction = new QAction(tr("&Quit"), this);
  connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
  this->addAction(quitAction);

}
