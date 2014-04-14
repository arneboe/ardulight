#pragma once
#include <QMenu>
#include <vector>
#include <memory>

class Light;
class LightController;
class QAction;

class TrayMenu : public QMenu
{
  Q_OBJECT
public:
  explicit TrayMenu(Light* light, QWidget* parent = NULL);

private:

  /**Display all controllers that are active */
  void updateMenuEntires();

  std::vector<std::unique_ptr<LightController> > controllers;
  QAction * quitAction;
};
