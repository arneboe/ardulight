#include "traymenu.h"
#include <QAction>
#include <QWidgetAction>
#include "light.h"
#include "LightController.h"
#include "examplecontroller.h"
#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>
TrayMenu::TrayMenu(std::shared_ptr<Light> light, QWidget* parent) :
  QMenu(parent), quitAction(new QAction("Quit", this)), activeController(-1)
{
  //FIXME this should not be connected to qApp.quit(). Instead it should
  //invoke a local slot which cleans up before quitting.
  connect(quitAction, SIGNAL(triggered()),qApp,SLOT(quit()));

  std::unique_ptr<LightController> example((LightController*) new ExampleController(light));

  controllers.push_back(std::move(example));

  //populate the controller selection submenu
  QMenu* controllerSelection = new QMenu("Select controller", this);
  this->addMenu(controllerSelection);
  for(const std::unique_ptr<LightController>& controller : controllers)
  {
    controllerSelection->addAction(controller->getName());
  }

  this->addSeparator();

  //add all actions
  for(const std::unique_ptr<LightController>& controller : controllers)
  {
    QWidgetAction* ctrlAction = controller->getMenuWidget();
    if(nullptr != ctrlAction)
    {
      this->addAction(ctrlAction);
      ctrlAction->setVisible(false);
    }
  }

  //set brightness action
  QSlider* brightnessSlider = new QSlider(nullptr);
  brightnessSlider->setOrientation(Qt::Horizontal);
  brightnessSlider->setMinimum(0);
  brightnessSlider->setMaximum(255);
  brightnessSlider->setValue(255);
  QLabel* brightnesslabel = new QLabel("Brightness", nullptr);
  QHBoxLayout* brightnessLayout = new QHBoxLayout(nullptr);
  brightnessLayout->addWidget(brightnesslabel);
  brightnessLayout->addWidget(brightnessSlider);
  QWidget* brightnessWidget = new QWidget(nullptr);
  brightnessWidget->setLayout(brightnessLayout);
  QWidgetAction* brigthnessAction = new QWidgetAction(nullptr);
  brigthnessAction->setDefaultWidget(brightnessWidget);

  connect(brightnessSlider, SIGNAL(valueChanged(int)),
                    this, SLOT(brightnessChanged(int)));
  this->addAction(brigthnessAction);
  this->addSeparator();
  this->addAction(quitAction);

  activateController(0);//FIXME should be loaded from settings?!
}

void TrayMenu::activateController(const int controllerIndex)
{
  Q_ASSERT(controllerIndex >= 0);
  Q_ASSERT(controllerIndex < controllers.size());
  if(activeController != -1)//is -1 if there has never been an active controller
  {
    controllers[activeController]->deactivate();
    controllers[activeController]->getMenuWidget()->setVisible(false);
  }
  activeController = controllerIndex;
  controllers[controllerIndex]->activate();
  controllers[controllerIndex]->getMenuWidget()->setVisible(true);
}

void TrayMenu::brightnessChanged(int newBrightness)
{
  Q_ASSERT(newBrightness >= 0);
  Q_ASSERT(newBrightness <= 255);

  if(activeController != -1)//if there is an active controller
  {
    controllers[activeController]->setBrightness(newBrightness);
  }
}
