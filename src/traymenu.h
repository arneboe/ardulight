#pragma once
#include <QMenu>

class Light;
class TrayMenu : public QMenu
{
  Q_OBJECT
public:
  explicit TrayMenu(Light* light, QWidget* parent = NULL);

signals:

public slots:
  void brightnessChanged(int newBrightness);

private:
  Light* pLight;
};
