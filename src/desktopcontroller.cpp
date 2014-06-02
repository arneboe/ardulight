#include "DesktopController.h"
#include <QMutexLocker>
#include <QWidgetAction>
#include "Screen.h"
#include "Light.h"
#include "Global.h"
#include <QDebug>

DesktopController::DesktopController() :
  active(false), brightness(255),
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

void DesktopController::activate(std::shared_ptr<Light> pLight)
{
  if(!active)
  {
    light = pLight;
    active = true;
    light->moveToThread(this); //take ownership of the light
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

QString DesktopController::getName()
{
  return "Ambilight";
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
      light->setColor(region.ledIndex, qRed(avg), qGreen(avg), qBlue(avg));
    }
    light->sendColors();

    const unsigned char tempBrightness = brightness;//copy to avoid race condition
    if(tempBrightness != currentBrightness)
    {
      currentBrightness = tempBrightness;
      light->setBrightness(tempBrightness);
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

  const int regionThickness = settings.value(thicknessSetting, 120).toInt();

  //left regions
  const int leftBottomIndex = settings.value(leftBottomSetting, 0).toInt();
  const int leftTopIndex = settings.value(leftTopSetting, 0).toInt();
  const int numLeftLeds = abs(leftBottomIndex - leftTopIndex) + 1; //+1 because both indices are inclusive
  const int pixelsPerLeftLed = screenHeight / numLeftLeds;
  //the amout of pixels that will be skipped at the top and bottom if resY is not
  //divisable by numLeftLeds.
  const int halfRestPixelsLeft = (screenHeight % numLeftLeds) / 2;
  const int leftIndexDirection = leftTopIndex > leftBottomIndex ? -1 : 1;

  for(int i = 0, ledIndex = leftTopIndex; i < numLeftLeds; ++i, ledIndex += leftIndexDirection)
  {
    const int regionX = 0;
    const int regionY = halfRestPixelsLeft + i * pixelsPerLeftLed;
    Region r;
    r.ledIndex = ledIndex;
    r.rect = QRect(regionX, regionY, regionThickness, pixelsPerLeftLed);
    regions.push_back(r);
    qDebug() << "LED " << r.ledIndex << ": x = " << r.rect.x() << ", y = " << r.rect.y() <<
                ", w = " << r.rect.width() << ", h = " << r.rect.height();
  }


  //right regions
  const int rightBottomIndex = settings.value(rightBottomSetting, 0).toInt();
  const int rightTopIndex = settings.value(rightTopSetting, 0).toInt();
  const int numRightLeds = abs(rightBottomIndex - rightTopIndex) + 1; //+1 because both indices are inclusive
  const int pixelsPerRightLed = screenHeight / numRightLeds;
  const int halfRestPixelsRight = (screenHeight % numRightLeds) / 2;
  const int rightIndexDirection = rightTopIndex > rightBottomIndex ? -1 : 1;

  for(int i = 0, ledIndex = rightTopIndex; i < numRightLeds; ++i, ledIndex += rightIndexDirection)
  {
    const int regionX = screenWidth - regionThickness;
    const int regionY = halfRestPixelsRight + i * pixelsPerRightLed;
    Region r;
    r.ledIndex = ledIndex;
    r.rect = QRect(regionX, regionY, regionThickness, pixelsPerRightLed);
    regions.push_back(r);
    qDebug() << "LED " << r.ledIndex << ": x = " << r.rect.x() << ", y = " << r.rect.y() <<
                ", w = " << r.rect.width() << ", h = " << r.rect.height();
  }


  //bottom regions
  const int bottomRightIndex = settings.value(bottomRightSetting, 0).toInt();
  const int bottomLeftIndex = settings.value(bottomLeftSetting, 0).toInt();
  const int numBottomLeds = abs(bottomRightIndex - bottomLeftIndex) + 1; //+1 because both indices are inclusive
  const int pixelsPerBottomLed = screenWidth / numBottomLeds;
  const int halfRestPixelsBottom = (screenWidth % numBottomLeds) / 2;
  const int bottomIndexDirection = bottomLeftIndex > bottomRightIndex ? -1 : 1;

  for(int i = 0, ledIndex = bottomLeftIndex; i < numBottomLeds; ++i, ledIndex += bottomIndexDirection)
  {
    const int regionX = halfRestPixelsBottom + i * pixelsPerBottomLed;
    const int regionY = screenHeight - regionThickness;
    Region r;
    r.ledIndex = ledIndex;
    r.rect = QRect(regionX, regionY, pixelsPerBottomLed, regionThickness);
    regions.push_back(r);
    qDebug() << "LED " << r.ledIndex << ": x = " << r.rect.x() << ", y = " << r.rect.y() <<
                ", w = " << r.rect.width() << ", h = " << r.rect.height();
  }


  //top regions
  const int topRightIndex = settings.value(topRightSetting, 0).toInt();
  const int topLeftIndex = settings.value(topLeftSetting, 0).toInt();
  const int numTopLeds = abs(topRightIndex - topLeftIndex) + 1; //+1 because both indices are inclusive
  const int pixelsPerTopLed = screenWidth / numTopLeds;
  const int halfRestPixelsTop = (screenWidth % numTopLeds) / 2;
  const int topIndexDirection = topLeftIndex > topRightIndex ? -1 : 1;

  for(int i = 0, ledIndex = topLeftIndex; i < numTopLeds; ++i, ledIndex += topIndexDirection)
  {
    const int regionX = halfRestPixelsTop + i * pixelsPerTopLed;
    const int regionY = 0;
    Region r;
    r.ledIndex = ledIndex;
    r.rect = QRect(regionX, regionY, pixelsPerBottomLed, regionThickness);
    regions.push_back(r);
    qDebug() << "LED " << r.ledIndex << ": x = " << r.rect.x() << ", y = " << r.rect.y() <<
                ", w = " << r.rect.width() << ", h = " << r.rect.height();
  }

}

