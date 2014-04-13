#include "ambilightmode.h"
#include "light.h"
#include <QImage>
#include <QPainter>
#include <QLabel>

AmbilightMode::AmbilightMode() : QThread(), pBuffer(NULL), stopThread(false)
{

}

AmbilightMode::~AmbilightMode()
{
  if(NULL != pBuffer)
  {
    delete[] pBuffer;
    pBuffer = NULL;
  }

}

void AmbilightMode::startMode(Light* light)
{
  light->moveToThread(this); //take ownership of the light
  pLight = light;
  stopThread = false;
  screen.initialize(0);
  generateRegions();
  pBuffer = new Screen::Pixel[screen.getHeight() * screen.getWidth()];
  QThread::start();
}

void AmbilightMode::stopMode()
{
  stopThread = true;
  QMutexLocker ml(&mutex); //wait for thread to finish
  delete[] pBuffer;
  pBuffer = NULL;
}

QString AmbilightMode::getName() const
{
  return QString("Ambilight");
}

void AmbilightMode::run()
{
  QMutexLocker ml(&mutex);
  while(!stopThread)
  {
    QCoreApplication::processEvents(); //process events to handle the slots
    screen.getScreenBitmap(pBuffer);
    {
      for(const Region& region : regions)
      {
        const Screen::Pixel avg = getAvgOfRegion(region.rect);
        pLight->setColor(region.ledIndex, qRed(avg), qGreen(avg), qBlue(avg));
      }
      pLight->sendColors();
    }
    QThread::msleep(5);
  }
}

Screen::Pixel AmbilightMode::getAvgOfRegion(const QRect& region) const
{
  unsigned long avgR = 0;
  unsigned long avgG = 0;
  unsigned long avgB = 0;
  const int width = screen.getWidth();
  for(int y = region.top(); y < region.bottom(); ++y)
  { //FIXME pBuffer should be a parameter
    for(int x = region.left(); x < region.right(); ++x)
    {//FIXME index based access is slow. use pointer magic
      Screen::Pixel p = pBuffer[width * y + x];
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

void AmbilightMode::generateRegions()
{
  regions.clear();

  const int regionWidth = 50; //FIXME parameter
  const int resX = screen.getWidth();
  const int resY = screen.getHeight();
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

