#include "colorpickercontroller.h"
#include <QWidget>
#include <QWidgetAction>
#include <QPushButton>
#include <QColorDialog>
#include <QColor>
#include "light.h"
#include <QThread>
#include <QMutexLocker>

ColorPickerController::ColorPickerController(std::shared_ptr<Light> light)
  : LightController(light, "Color Picker"), active(false),
    color("red")//FIXME should be loaded from file
{
  QPushButton* button = new QPushButton("Pick color");
  connect(button, SIGNAL(clicked(bool)), this, SLOT(buttonClicked(bool)));

  pWidget = new QWidgetAction(nullptr);
  pWidget->setDefaultWidget(button);
  pPicker = new QColorDialog();
  connect(pPicker, SIGNAL(currentColorChanged(QColor)), this, SLOT(colorChanged(QColor)));
  connect(pPicker, SIGNAL(rejected()), this, SLOT(colorRejected()));
}

void ColorPickerController::activate()
{
  if(!active)
  {
    pLight->moveToThread(this);
    active = true;
    QThread::start();
  }
}

void ColorPickerController::deactivate()
{
  active = false;
  QMutexLocker ml(&threadActive);//wait for the run method to finish
}

bool ColorPickerController::isActive() const
{
  return active;
}

void ColorPickerController::setBrightness(const unsigned char value)
{
  //FIXME
  qWarning("setBrightness in color picker not implemented");
}

QWidgetAction *ColorPickerController::getMenuWidget()
{
  Q_ASSERT(nullptr != pWidget);
  return pWidget;
}

void ColorPickerController::buttonClicked(bool)
{
  if(active)
  {
    oldColor = color;
    pPicker->open();
  }
}

void ColorPickerController::colorChanged(const QColor& col)
{
  if(active)
  {
    color = col;
  }
}

void ColorPickerController::colorRejected()
{
  color = oldColor;
}

void ColorPickerController::run()
{
  QMutexLocker ml(&threadActive);
  QColor currentColor;

  while(active)
  {
    QColor tempColor = color; //copy to avoid race condition
    if(tempColor != currentColor)
    {
      currentColor = tempColor;
      pLight->setAllColors((unsigned char) tempColor.red(),
                           (unsigned char) tempColor.green(),
                           (unsigned char) tempColor.blue());
      pLight->sendColors();
    }
    QThread::msleep(30);
  }
}
