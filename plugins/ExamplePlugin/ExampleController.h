#pragma once
#include <LightController.h>
#include <QThread>
#include <QMutex>
#include <QtPlugin>

/**
 * @brief A simple light controller that blinks randomly
 */
class ExampleController : public QThread, LightController
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID LightController_iid)
  Q_INTERFACES(LightController)
public:
  ExampleController();

public:
  void activate(std::shared_ptr<ILight> pLight);
  void deactivate();
  bool isActive() const;
  void setBrightness(const unsigned char value);
  QWidgetAction* getMenuWidget();
  QString getName();

private slots:
  void speedChanged(int newSpeed);

private:
  void run();

private: //attributes
  bool active;
  int speed;
  QMutex threadActive;
  unsigned char brightness; //the brightness that should be set
  std::shared_ptr<ILight> light;
  /**When this controller is activated it remebers the original owner of the Light
   * to restore the ownership when it finishes execution */
  QThread* previousLightOwner;

};
