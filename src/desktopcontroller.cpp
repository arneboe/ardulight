#include "DesktopController.h"
#include <QMutexLocker>
#include <QWidgetAction>
#include "Screen.h"
#include "Light.h"
#include "Global.h"
#include <QDebug>

DesktopController::DesktopController(std::shared_ptr<Light> light) :
  LightController(light, "Ambilight"), active(false), brightness(255),
  settings(Global::getInstance().getSettings()),
  refreshRateSetting("Controller/Desktop/RefreshRate"),
  bottomLeftSetting("Controller/Desktop/Leds/BottomLeft"),
  bottomRightSetting("Controller/Desktop/Leds/BottomRight"),
  leftBottomSetting("Controller/Desktop/Leds/LeftBottom"),
  leftTopSetting("Controller/Desktop/Leds/leftTop"),
  topLeftSetting("Controller/Desktop/Leds/TopLeft"),
  topRightSetting("Controller/Desktop/Leds/TopRight"),
  rightBottomSetting("Controller/Desktop/Leds/RightBottom"),
  rightTopSetting("Controller/Desktop/Leds/RightTop"),
  thicknessSetting("Controller/Desktop/Leds/RegionThickness")
{
}

void DesktopController::activate()
{
  if(!active)
  {
    active = true;
    pLight->moveToThread(this); //take ownership of the light
    QThread::start();
  }
}

void DesktopController::deactivate()
{
  active = false;
  QMutexLocker waitForThread(&threadActive);
}

bool DesktopController::isActive() const
{
  return active;
}

void DesktopController::setBrightness(const unsigned char value)
{
  brightness = value;
}

QWidgetAction *DesktopController::getMenuWidget()
{
  //FIXME
  qWarning("dummy implementation");
  return new QWidgetAction(NULL);
}

void DesktopController::run()
{
  QMutexLocker lock(&threadActive);
  unsigned char currentBrightness = 255;
  //setup screen
  Screen screen;
  screen.initialize(0); //FIXME load from settings
  //FIXME initialize() can fail(returns a boolean)
  //FIXME this works as long as the pixel format from Screen is always ARGB
  std::vector<QRgb> buffer;
  buffer.resize(screen.getHeight() * screen.getWidth());
  std::vector<Region> regions;
  generateRegions(regions, screen.getWidth(), screen.getHeight());

  while(active)
  {
    screen.getScreenBitmap(buffer.data());
    for(const Region& region : regions)
    {
      const QRgb avg = getAvgOfRegion(region.rect, buffer.data(), screen.getWidth());
      pLight->setColor(region.ledIndex, qRed(avg), qGreen(avg), qBlue(avg));
    }
    pLight->sendColors();

    const unsigned char tempBrightness = brightness;//copy to avoid race condition
    if(tempBrightness != currentBrightness)
    {
      currentBrightness = tempBrightness;
      pLight->setBrightness(tempBrightness);
    }

    QThread::msleep(5); //FIXME use refreshRate from settings
  }
}

QRgb DesktopController::getAvgOfRegion(const QRect& region, QRgb* pBuffer, const int screenWidth) const
{
  unsigned long avgR = 0;
  unsigned long avgG = 0;
  unsigned long avgB = 0;
  const int width = screenWidth;
  for(int y = region.top(); y < region.bottom(); ++y)
  {
    for(int x = region.left(); x < region.right(); ++x)
    {
      QRgb p = pBuffer[width * y + x];
      avgR += qRed(p);
      avgG += qGreen(p);
      avgB += qBlue(p);
    }
  }
  const int count = region.width() * region.height();
  avgR /= count;
  avgG /= count;
  avgB /= count;
  Q_ASSERT(avgR <= 255);
  Q_ASSERT(avgG <= 255);
  Q_ASSERT(avgB <= 255);
  return qRgb(avgR, avgG, avgB);
}

void DesktopController::generateRegions(std::vector<Region>& regions, const int screenWidth, const int screenHeight) const
{
  //FIXME this method contains a lot of duplicate code
  regions.clear();

  const int resY = screenHeight;
  const int regionThickness = settings.value(thicknessSetting, 120).toInt();

  //left regions
  const int leftBottomIndex = settings.value(leftBottomSetting, 0).toInt();
  const int leftTopIndex = settings.value(leftTopSetting, 0).toInt();
  const int numLeftLeds = abs(leftBottomIndex - leftTopIndex) + 1; //+1 because both indices are inclusive
  const int pixelsPerLeftLed = resY / numLeftLeds;
  //the amout of pixels that will be skipped at the top and bottom if resY is not
  //divisable by numLeftLeds.
  const int halfRestPixelsLeft = (resY % numLeftLeds) / 2;
  const int leftIndexDirection = leftTopIndex > leftBottomIndex ? -1 : 1;

  for(int i = 0, ledIndex = leftTopIndex; i < numLeftLeds; ++i, ledIndex += leftIndexDirection)
  {
    const int regionX = 0;
    const int regionY = halfRestPixelsLeft + i * pixelsPerLeftLed;
    Region r;
    r.ledIndex = ledIndex;
    r.rect = QRect(regionX, regionY, regionThickness, pixelsPerLeftLed);
    regions.push_back(r);
    qDebug() << "LED " << r.ledIndex << ": x = " << regionX << ", y = " << regionY <<
                ", w = " << regionThickness << ", h = " << pixelsPerLeftLed;
  }


  //right regions
  const int rightBottomIndex = settings.value(rightBottomSetting, 0).toInt();
  const int rightTopIndex = settings.value(rightTopSetting, 0).toInt();
  const int numRightLeds = abs(rightBottomIndex - rightTopIndex) + 1; //+1 because both indices are inclusive
  const int pixelsPerRightLed = resY / numRightLeds;
  const int halfRestPixelsRight = (resY % numRightLeds) / 2;
  const int rightIndexDirection = rightTopIndex > rightBottomIndex ? -1 : 1;

  for(int i = 0, ledIndex = rightTopIndex; i < numRightLeds; ++i, ledIndex += rightIndexDirection)
  {
    const int regionX = screenWidth - regionThickness;
    const int regionY = halfRestPixelsRight + i * pixelsPerRightLed;
    Region r;
    r.ledIndex = ledIndex;
    r.rect = QRect(regionX, regionY, regionThickness, pixelsPerRightLed);
    regions.push_back(r);
    qDebug() << "LED " << r.ledIndex << ": x = " << regionX << ", y = " << regionY <<
                ", w = " << regionThickness << ", h = " << pixelsPerLeftLed;
  }

}

