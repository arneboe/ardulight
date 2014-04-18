#pragma once
#include <memory>
class Light;
class QWidgetAction;
/**
 * The base class of all light controllers.
 */
class LightController
{
public:
  LightController(std::shared_ptr<Light> light) : pLight(light){}

  /**
   * Start execution of this light controller.
   * This method should never block.
   * Once a controller is active it is allowed to control the light.
   *
   * @note The light might be owned by a different thread. The controller
   *       should move it to the correct thread upon activation.
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
   * Sets the brightness of the light.
   * This method is called when this controller is active and the user wants
   * to change the brightness. It is the responsibility of the controller to
   * forward the command to the light because the light may only be controlled
   * by one controller at a time.
   */
  virtual void setBrightness(const unsigned char value) = 0;

  /**
   * While active this widget is shown in the tray menu.
   * It can be used to configure the controller. 
   */
  virtual QWidgetAction* getMenuWidget()
  {
    return nullptr;
  }

protected:
  std::shared_ptr<Light> pLight; /**<The light that is controlled by this controller */
};
