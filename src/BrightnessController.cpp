#include "BrightnessController.h"
#include "light.h"
#include <QWidget>
#include <QSlider>
#include <QVBoxLayout>
#include <QLabel>
#include <QWidgetAction>

BrightnessController::BrightnessController(Light *light) : LightController(light),
                                                           active(false)
{
  Q_ASSERT(NULL != light);

  QSlider* brightnessSlider = new QSlider(nullptr);
  brightnessSlider->setOrientation(Qt::Horizontal);
  brightnessSlider->setMinimum(0);
  brightnessSlider->setMaximum(255);
  QLabel* brightnessLabel = new QLabel("Brightness", nullptr);
  QVBoxLayout* brightnessLayout = new QVBoxLayout(nullptr);
  brightnessLayout->addWidget(brightnessLabel);
  brightnessLayout->addWidget(brightnessSlider);
  QWidget* widget = new QWidget(nullptr);
  widget->setLayout(brightnessLayout);
  pWidget = new QWidgetAction(nullptr);
  pWidget->setDefaultWidget(widget);


  connect(brightnessSlider, SIGNAL(valueChanged(int)),
                     this, SLOT(brightnessChanged(int)));
}

void BrightnessController::activate()
{
  active = true;
}

void BrightnessController::deactivate()
{
  active = false;
}

bool BrightnessController::isActive() const
{
  return active;
}

QWidgetAction *BrightnessController::getMenuWidget()
{
  Q_ASSERT(pWidget != NULL);
  return pWidget;
}

void BrightnessController::brightnessChanged(int newBrightness)
{
  if(active)
  {
    Q_ASSERT(pLight != NULL);
    pLight->setBrightness(newBrightness);
  }
}
