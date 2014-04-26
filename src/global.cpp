#include "Global.h"
#include <QApplication>
Global::Global() : settings("config.ini", QSettings::IniFormat)
{}

QSettings& Global::getSettings()
{
  return settings;
}
