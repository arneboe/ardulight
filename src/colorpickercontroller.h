#pragma once
#include "LightController.h"
#include <QThread>
#include <QMutex>
#include <QColor>

class QColorDialog;

/**
 * Lets the user set the color manually using a color picker
 * with live preview.
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

  bool active;
  QWidgetAction* pWidget;
  QMutex threadActive;
  QColor color;//when active this variable is monitored by the thread and any change is transmitted to the light
  QColor oldColor;//Is used to reset the color if the user clicks 'cancel' in the dialog
  QColorDialog* pPicker;
};
