#pragma once
#include <QThread>
#include <QMutex>
#include "Screen.h"
#include "imode.h"
#include <vector>
#include <QRect>
#include "light.h"
class AmbilightMode : public QThread, public IMode
{
  Q_OBJECT
public:
  AmbilightMode();
  ~AmbilightMode();
  virtual void startMode(Light* light); //starts the Mode, may not block.
  virtual void stopMode();//stops the Mode, may block until operation has finished.
  virtual QString getName() const;
  void run();
private:

  struct Region
  {
    int ledIndex;
    QRect rect;
  };

  Screen::Pixel getAvgOfRegion(const QRect &region) const;
  void generateRegions();

  QMutex mutex; /**<Is locked while thread is running */
  Screen screen;
  Screen::Pixel* pBuffer;
  bool stopThread;
  std::vector<Region> regions; /**<Contains a screen region for each led*/
  Light* pLight;//needs to be created in the thread, otherwise serial port does not work

};
