#pragma once

class Light;
class QWidget;
/**
 * The base class of all light controllers.
 */
class LightController
{
  LightController(Light* light) : pLight(light){}

  /**
   * Start execution of this light controller.
   * This method should never block.
   * Once a controller is active it is allowed to control the light.
   */
  virtual void activate() = 0;

  /**
   * Stop execution if this light controller.
   * This method may block for a short time, i.e. the time it takes to
   * savely stop the controller.
   *
   * Once a controller has been deactivated it is no longer allowed
   * to send any messages to the light.
   */
  virtual void deactivate() = 0;

  /**
   * While active this widget is shown in the tray menu.
   * It can be used to configure the controller.
   */
  virtual QWidget* getMenuWidget()
  {
    return NULL;
  }

protected:
  Light* pLight; /**<The light that is controlled by this controller */
};
