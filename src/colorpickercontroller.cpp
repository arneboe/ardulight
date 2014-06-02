#include "ColorPickerController.h"
#include <QWidget>
#include <QWidgetAction>
#include <QPushButton>
#include <QColorDialog>
#include <QColor>
#include "Light.h"
#include <QThread>
#include <QMutexLocker>
#include "Global.h"

ColorPickerController::ColorPickerController()
  : active(false),
    color("red"),//if it remains red something went wrong :D
    settings(Global::getInstance().getSettings()),
    redSetting("Controller/ColorPicker/Color/Red"),
    greenSetting("Controller/ColorPicker/Color/Green"),
    blueSetting("Controller/ColorPicker/Color/Blue")
{
  const int red = settings.value(redSetting, 255).toInt();
  const int green = settings.value(greenSetting, 255).toInt();
  const int blue  = settings.value(blueSetting, 255).toInt();
  color.setRgb(red, green, blue);

  pPicker = new QColorDialog();
  connect(pPicker, SIGNAL(currentColorChanged(QColor)), this, SLOT(colorChanged(QColor)));
  connect(pPicker, SIGNAL(rejected()), this, SLOT(colorRejected()));
  connect(pPicker, SIGNAL(accepted()), this, SLOT(colorAccepted()));
}

void ColorPickerController::activate(std::shared_ptr<Light> pLight)
{
  if(!active)
  {
    light = pLight;
    light->moveToThread(this);
    active = true;
    QThread::start();
    //signal once to set the last known color
    signalColorChanged.release();
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

QString ColorPickerController::getName()
{
  return "Color Picker";
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

void ColorPickerController::colorAccepted()
{
  settings.setValue(redSetting, color.red());
  settings.setValue(greenSetting, color.green());
  settings.setValue(blueSetting, color.blue());
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
      light->setAllColors((unsigned char) tempColor.red(),
                           (unsigned char) tempColor.green(),
                           (unsigned char) tempColor.blue());
      light->sendColors();
    }
  }
}
