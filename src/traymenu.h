#pragma once
#include <QMenu>
#include <vector>
#include <memory>

class ILight;
class LightController;
class QAction;
class TrayMenu : public QMenu
{
  Q_OBJECT
public:
  explicit TrayMenu(std::shared_ptr<ILight> light, QWidget* parent = NULL);

private:

  /**Display all controllers that are active */
  void updateMenuEntires();
  void activateController(const int controllerIndex);
private slots:
  void brightnessChanged(int newBrightness);
  void controllerSelected(QAction* action);

private:

  /**Loads all light controllers */
  void loadPlugins();

  std::vector<std::unique_ptr<LightController> > controllers;
  QAction * quitAction;
  QMenu* controllerSelection;/**<The submenu that is used to select controllers */
  int activeController; /**< Index of the currently active controller */
  std::shared_ptr<ILight> light;
};
