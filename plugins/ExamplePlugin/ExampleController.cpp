#include "ExampleController.h"
#include <QMutexLocker>
#include <Light.h>
#include <cstdlib>
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>
#include <QWidget>
#include <QWidgetAction>

ExampleController::ExampleController() :
  active(false), speed(500), brightness(255)
{}

void ExampleController::activate(std::shared_ptr<ILight> pLight)
{
  if(!active)
  {
    light = pLight;
    previousLightOwner = light->thread();
    light->moveToThread(this);//This only works if the calling thread is the owner of light
    active = true;
    QThread::start();
  }
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
  brightness = value;
}

QWidgetAction *ExampleController::getMenuWidget()
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
  QWidgetAction* action = new QWidgetAction(nullptr);
  action->setDefaultWidget(widget);

  connect(slider, SIGNAL(valueChanged(int)),
                    this, SLOT(speedChanged(int)));
  return action;
}

QString ExampleController::getName()
{
  return "Example";
}

void ExampleController::speedChanged(int newSpeed)
{
  speed = newSpeed;
}

void ExampleController::run()
{
  QMutexLocker ml(&threadActive);
  unsigned char currentBrightness = 0;
  if(nullptr != light.get())
  {
    while(active)
    {
      const unsigned char brightnessCopy = brightness; //to avoid race condition
      if(currentBrightness != brightnessCopy)
      {
        currentBrightness = brightnessCopy;
        light->setBrightness(brightnessCopy);
      }
      const unsigned char r = rand() % 255;
      const unsigned char g = rand() % 255;
      const unsigned char b = rand() % 255;
      light->setAllColors(r, g, b);
      light->sendColors();
      QThread::msleep(speed);
    }

    //Move the Light back to the main thread.
    //This is important otherwise other plugins might not be
    //able to use it
    light->moveToThread(previousLightOwner);
  }
  else
  {
    qWarning() << "ExampleController: light is NULL";
  }
}

