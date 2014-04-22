#include "desktopcontroller.h"
#include <QMutexLocker>
#include <QWidgetAction>
#include "Screen.h"
#include "light.h"

DesktopController::DesktopController(std::shared_ptr<Light> light) :
  LightController(light, "Ambilight"), active(false), brightness(255)
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
  QRgb* buffer = new QRgb[screen.getHeight() * screen.getWidth()];
  std::vector<Region> regions;
  generateRegions(regions, screen.getWidth(), screen.getHeight());

  while(active)
  {
    screen.getScreenBitmap(buffer);
    for(const Region& region : regions)
    {
      const QRgb avg = getAvgOfRegion(region.rect, buffer, screen.getWidth());
      pLight->setColor(region.ledIndex, qRed(avg), qGreen(avg), qBlue(avg));
    }
    pLight->sendColors();

    const unsigned char tempBrightness = brightness;//copy to avoid race condition
    if(tempBrightness != currentBrightness)
    {
      currentBrightness = tempBrightness;
      pLight->setBrightness(tempBrightness);
    }

    QThread::msleep(5);
  }
  delete buffer; //FIXME use scoped destruction
}

QRgb DesktopController::getAvgOfRegion(const QRect& region, QRgb* pBuffer, const int screenWidth) const
{
  unsigned long avgR = 0;
  unsigned long avgG = 0;
  unsigned long avgB = 0;
  const int width = screenWidth;
  for(int y = region.top(); y < region.bottom(); ++y)
  { //FIXME pBuffer should be a parameter
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
  //FIXME this should be loaded from config file
  regions.clear();

  const int regionWidth = 50; //FIXME parameter
  const int resX = screenWidth;
  const int resY = screenHeight;
  const int horTotalWidth = resX - 2 * regionWidth;
  const int horRegionCount = 20; //FIXME parameter
  const int horRegionWidth = horTotalWidth / horRegionCount;
  const int vertTotalWidth = resY - 2 * regionWidth;
  const int vertRegionCount = 10;
  const int vertRegionWidth = vertTotalWidth / vertRegionCount;

  //top regions
  for(int rIndex = 30, i = 0; i < horRegionCount; ++i, ++rIndex) //FIXME hard coded led index
  {
    const int regionX = regionWidth + i * horRegionWidth;
    const int regionY = 0;
    Region r;
    r.ledIndex = rIndex;
    r.rect = QRect(regionX, regionY, horRegionWidth, regionWidth);
    regions.push_back(r);
  }

  //bottom regions
  for(int rIndex = 19, i = 0; i < horRegionCount; ++i, --rIndex)//FIXME hard coded led index
  {
    const int regionX = regionWidth + i * horRegionWidth;
    const int regionY = resY - regionWidth;
    Region r;
    r.ledIndex = rIndex;
    r.rect = QRect(regionX, regionY, horRegionWidth, regionWidth);
    regions.push_back(r);
  }

  //left regions
  for(int rIndex = 29, i = 0; i < vertRegionCount; ++i, --rIndex)//FIXME hard coded led index
  {
    const int regionX = 0;
    const int regionY = regionWidth + i * vertRegionWidth;
    Region r;
    r.ledIndex = rIndex;
    r.rect = QRect(regionX, regionY, regionWidth, vertRegionWidth);
    regions.push_back(r);
  }

  //right regions
  for(int rIndex = 50, i = 0; i < vertRegionCount; ++i, ++rIndex)//FIXME hard coded led index
  {
    const int regionX = resX - regionWidth;
    const int regionY = regionWidth + i * vertRegionWidth;
    Region r;
    r.ledIndex = rIndex;
    r.rect = QRect(regionX, regionY, regionWidth, vertRegionWidth);
    regions.push_back(r);
  }
}

