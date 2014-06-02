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
  ExampleController();

public:
  void activate(std::shared_ptr<Light> pLight);
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
  std::shared_ptr<Light> light;

};
