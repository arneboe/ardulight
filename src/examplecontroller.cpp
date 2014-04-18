#include "examplecontroller.h"
#include <QMutexLocker>
#include "light.h"
#include <cstdlib>
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>
#include <QWidget>
#include <QWidgetAction>

ExampleController::ExampleController(std::shared_ptr<Light> light) :
  LightController(light), active(false), speed(500)
{
  QSlider* slider = new QSlider(nullptr);
  slider->setOrientation(Qt::Horizontal);
  slider->setMinimum(100);
  slider->setMaximum(1000);
  slider->setValue(speed);
  QLabel* label = new QLabel("Speed", nullptr);
  QHBoxLayout* layout = new QHBoxLayout(nullptr);
  layout->addWidget(label);
  layout->addWidget(slider);
  QWidget* widget = new QWidget(nullptr);
  widget->setLayout(layout);
  pWidget = new QWidgetAction(nullptr);
  pWidget->setDefaultWidget(widget);

  connect(slider, SIGNAL(valueChanged(int)),
                    this, SLOT(speedChanged(int)));
}

void ExampleController::activate()
{
  pLight->moveToThread(this); //take ownership of the light
  active = true;
  QThread::start();
}

void ExampleController::deactivate()
{
  active = false;
  QMutexLocker ml(&threadActive); //wait for the run method to finish
}

bool ExampleController::isActive() const
{
  return active;
}

void ExampleController::setBrightness(const unsigned char value)
{
  //TODO
}

QWidgetAction *ExampleController::getMenuWidget()
{
  Q_ASSERT(nullptr != pWidget);
  return pWidget;
}

void ExampleController::speedChanged(int newSpeed)
{
  speed = newSpeed;
}

void ExampleController::run()
{
  QMutexLocker ml(&threadActive);
  while(active)
  {
    const unsigned char r = rand() % 255;
    const unsigned char g = rand() % 255;
    const unsigned char b = rand() % 255;
    pLight->setAllColors(r, g, b);
    pLight->sendColors();
    QThread::msleep(speed);
  }
}

