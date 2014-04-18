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
  explicit TrayMenu(std::shared_ptr<Light> light, QWidget* parent = NULL);

private:

  /**Display all controllers that are active */
  void updateMenuEntires();
  void activateController(const int controllerIndex);
private slots:
  void brightnessChanged(int newBrightness);
  void controllerSelected(QAction* action);

private:
  std::vector<std::unique_ptr<LightController> > controllers;
  QAction * quitAction;
  QMenu* controllerSelection;/**<The submenu that is used to select controllers */
  int activeController; /**< Index of the currently active controller */
};
