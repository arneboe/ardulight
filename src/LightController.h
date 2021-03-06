#pragma once
#include <memory>
#include <QString>
#include <QtPlugin>
#include "ILight.h"

class QWidgetAction;
class QThread;

/**
 * The base class of all light controllers.
 * @note each controller should also be a QThread at the same time!
 */
class LightController
{
public:

  /**
   * Start execution of this light controller.
   * This method should never block.
   * Once a controller is active it is allowed to control the light.
   *
   * @note The light might be owned by a different thread. The controller
   *       should move it to the correct thread upon activation.
   */
  virtual void activate(std::shared_ptr<ILight> pLight) = 0;

  /**
   * Stop execution of this light controller.
   * This method may block for a short time, i.e. the time it takes to
   * savely stop the controller.
   *
   * Once a controller has been deactivated it is no longer allowed
   * to send any messages to the light.
   */
  virtual void deactivate() = 0;

  virtual bool isActive() const = 0;

  /**
   * Sets the brightness of the light.
   * This method is called when this controller is active and the user wants
   * to change the brightness. It is the responsibility of the controller to
   * forward the command to the light because the light may only be controlled
   * by one controller at a time.
   */
  virtual void setBrightness(const unsigned char value) = 0;

  virtual QString getName()= 0;

  /**
   * While active this widget is shown in the tray menu.
   * It can be used to configure the controller. 
   * @note Always return a new (!!!) object from this method.
   *       It will be destroyed when the controller is deactivated.
   */
  virtual QWidgetAction* getMenuWidget() = 0;

};

#define LightController_iid "de.rnae.LightController_V1" //note: the version string will increase every time the interface changes
Q_DECLARE_INTERFACE(LightController, LightController_iid)
