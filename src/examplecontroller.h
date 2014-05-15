#pragma once
#include "LightController.h"
#include <QThread>
#include <QMutex>

/**
 * @brief A simple light controller that blinks randomly
 */
class ExampleController : public QThread, LightController
{
  Q_OBJECT;
public:
  ExampleController(std::shared_ptr<Light> light);

public:
  void activate();
  void deactivate();
  bool isActive() const;
  void setBrightness(const unsigned char value);
  QWidgetAction* getMenuWidget();

private slots:
  void speedChanged(int newSpeed);

private:
  void run();

private: //attributes
  bool active;
  int speed;
  QMutex threadActive;
  unsigned char brightness; //the brightness that should be set


};
