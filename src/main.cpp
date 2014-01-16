#include "mainwindow.h"
#include <QApplication>
#include "DirectX.h"
#include <QLabel>
#include <QPixmap>
#include <QPainter>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  Screen x;
  x.initialize(1);
  Screen::Pixel* pData = new Screen::Pixel[x.getHeight() * x.getWidth()];
  x.getScreenBitmap(pData);
  QImage img(x.getWidth(), x.getHeight(),QImage::Format_ARGB32);
  for(int h = 0; h < x.getHeight(); ++h)
  {
    for(int w = 0; w < x.getWidth(); ++w, ++pData)
    {
      img.setPixel(w, h, *((unsigned int*)pData));
    }
  }
  QLabel myLabel;
  myLabel.setPixmap(QPixmap::fromImage(img));
  myLabel.show();

  return a.exec();
  delete[] pData;
}
