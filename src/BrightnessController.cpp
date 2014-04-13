#include "BrightnessController.h"
#include "light.h"
#include <QWidget>
#include <QSlider>
#include <QVBoxLayout>
#include <QLabel>

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
  pWidget = new QWidget(nullptr);
  pWidget->setLayout(brightnessLayout);

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

QWidget *BrightnessController::getMenuWidget()
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
