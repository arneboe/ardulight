#pragma once

class Light;
class QWidgetAction;
/**
 * The base class of all light controllers.
 */
class LightController
{
public:
  LightController(Light* light) : pLight(light){}

  /**
   * Start execution of this light controller.
   * This method should never block.
   * Once a controller is active it is allowed to control the light.
   */
  virtual void activate() = 0;

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
   * While active this widget is shown in the tray menu.
   * It can be used to configure the controller. 
   */
  virtual QWidgetAction* getMenuWidget()
  {
    return nullptr;
  }

protected:
  Light* pLight; /**<The light that is controlled by this controller */
};
