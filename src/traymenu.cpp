#include "traymenu.h"
#include <QAction>
#include <QWidgetAction>
#include "light.h"
#include "LightController.h"
#include "examplecontroller.h"
TrayMenu::TrayMenu(std::shared_ptr<Light> light, QWidget* parent) :
  QMenu(parent), quitAction(new QAction("Quit", this))
{
  //FIXME this should not be connected to qApp.quit(). Instead it should
  //invoke a local slot which cleans up before quitting.
  connect(quitAction, SIGNAL(triggered()),qApp,SLOT(quit()));

  std::unique_ptr<LightController> example((LightController*) new ExampleController(light));

  controllers.push_back(std::move(example));
  controllers.back()->activate(); //brightness controll is always active

  //add all actions
  for(const std::unique_ptr<LightController>& controller : controllers)
  {
    QWidgetAction* ctrlAction = controller->getMenuWidget();
    if(nullptr != ctrlAction)
    {
      this->addAction(ctrlAction);
    }
  }
  this->addAction(quitAction);
  updateMenuEntires();
}

void TrayMenu::updateMenuEntires()
{
  for(const std::unique_ptr<LightController>& controller : controllers)
  {
    QWidgetAction* ctrlAction = controller->getMenuWidget();
    Q_ASSERT(nullptr != ctrlAction);
    ctrlAction->setVisible(controller->isActive());
  }
}
