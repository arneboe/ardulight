#include "Global.h"
#include <QApplication>
#include <QDir>
#include <QDebug>
Global::Global() : settings(QDir(qApp->applicationDirPath()).absoluteFilePath("config.ini") , QSettings::IniFormat)
{
  qDebug() << "Loaded settings from: " << settings.fileName();
}

QSettings& Global::getSettings()
{
  return settings;
}
