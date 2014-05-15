#include "ColorPickerController.h"
#include <QWidget>
#include <QWidgetAction>
#include <QPushButton>
#include <QColorDialog>
#include <QColor>
#include "Light.h"
#include <QThread>
#include <QMutexLocker>

ColorPickerController::ColorPickerController(std::shared_ptr<Light> light)
  : LightController(light, "Color Picker"), active(false),
    color("red")//FIXME should be loaded from file
{
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
  signalColorChanged.release();//wake up thread
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
  QPushButton* button = new QPushButton("Pick color");
  connect(button, SIGNAL(clicked(bool)), this, SLOT(buttonClicked(bool)));

  QWidgetAction* widget = new QWidgetAction(nullptr);
  widget->setDefaultWidget(button);
  return widget;
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
    signalColorChanged.release();//tell thread that the color changed
  }
}

void ColorPickerController::colorRejected()
{
  color = oldColor;
  signalColorChanged.release();
}

void ColorPickerController::run()
{
  QMutexLocker ml(&threadActive);
  QColor currentColor;

  while(active)
  {
    signalColorChanged.acquire();//wait for color changed
    //note: If the user sets multiple colors in short time intervals
    //      we are going to miss some of them.
    //      However that does not matter because the user is only interested
    //      in the last color :)
    if(active)
    {//This check is necessary because the sema will also be released when
     //stopping the thread.
      QColor tempColor = color; //copy to avoid mixing two consectuive colors :)
      pLight->setAllColors((unsigned char) tempColor.red(),
                           (unsigned char) tempColor.green(),
                           (unsigned char) tempColor.blue());
      pLight->sendColors();
    }
  }
}
