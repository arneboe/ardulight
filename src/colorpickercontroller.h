#pragma once
#include "LightController.h"
#include <QThread>
#include <QMutex>
#include <QColor>
#include <QSemaphore>

class QColorDialog;

/**
 * Lets the user set the color manually using a color picker
 * with live preview.
 *
 * Concept:
 * The worker thread is sleeping on a semaphore.
 * Everytime the user picks a new color the attribute 'color' is updated.
 * Afterwards the sema is released and the worker thread sends the new color
 * to the light.
 *
 * @note
 *   The race condition between setting the color and sending it to the light
 *   is ignored because the user generally only cares about the last color.
 *
 *   Due to the race condition we might occasionally send the same color
 *   multiple times but that doesnt matter because this controller is not
 *   performance critical and simple clean code is more important than a little
 *   useless traffic on the bus :)
 */
class ColorPickerController : public QThread, LightController
{
   Q_OBJECT;
public:
  ColorPickerController(std::shared_ptr<Light> light);

  // LightController interface
public:
  void activate();
  void deactivate();
  bool isActive() const;
  void setBrightness(const unsigned char value);
  QWidgetAction *getMenuWidget();

private slots:
  void buttonClicked(bool checked);

  /*Is called when the color picker is open and the color has been changed */
  void colorChanged(const QColor& col);

  /*Called when the user hits 'cancel'.
   *Resets the color back to its original state*/
  void colorRejected();

private:
  void run();
  /*Sets the color and wakes up the worker thread */
  void setColor(const QColor& col);

  bool active;
  QWidgetAction* pWidget;
  QMutex threadActive;//is locked while the thread is active
  QColor color;//when active this variable is monitored by the thread and any change is transmitted to the light
  QColor oldColor;//Is used to reset the color if the user clicks 'cancel' in the dialog
  QColorDialog* pPicker;//The color picker dialog that is used
  QSemaphore signalColorChanged; //used to wake up the worker thread when the color changes
};
