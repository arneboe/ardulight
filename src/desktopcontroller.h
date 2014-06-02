#pragma once
#include "LightController.h"
#include <QThread>
#include <Memory>
#include <QMutex>
#include <QRgb>
#include <QRect>

class Light;
class QRect;
class Pixel;
class QSettings;
/**
 * Mirrors the desktop border colors
 */
class DesktopController : public QThread, LightController
{
public:
  DesktopController(std::shared_ptr<Light> light);
  void activate();
  void deactivate();
  bool isActive() const;
  void setBrightness(const unsigned char value);
  QWidgetAction *getMenuWidget();

private:
  struct Region
  {
    int ledIndex;
    QRect rect;
  };

  void run();
  QRgb getAvgOfRegion(const QRect& region, QRgb* pBuffer, const int screenWidth) const;
  void generateRegions(std::vector<Region>& regions, const int screenWidth, const int screenHeight) const;

  bool active;
  QMutex threadActive;
  unsigned char brightness; //the brightness that should be set

  QSettings& settings;
  const QString refreshRateSetting;
  const QString bottomLeftSetting;
  const QString bottomRightSetting;
  const QString leftBottomSetting;
  const QString leftTopSetting;
  const QString topLeftSetting;
  const QString topRightSetting;
  const QString rightBottomSetting;
  const QString rightTopSetting;
  const QString thicknessSetting;
};

