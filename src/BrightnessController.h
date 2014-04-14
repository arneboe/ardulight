#pragma once
#include "LightController.h"
#include <QObject>

class Light;
class QWidget;

/**
 * @brief A simple controller that enables the user to change the
 *        brightness.
 */
class BrightnessController : public QObject,  LightController
{
  Q_OBJECT;
public:
  BrightnessController(Light* light);

  virtual void activate();
  virtual void deactivate();
  virtual bool isActive() const;
  virtual QWidgetAction* getMenuWidget();
private slots:
  void brightnessChanged(int newBrightness);

private:
  QWidgetAction* pWidget;
  bool active; /**< True if the controller is currently active*/
};
