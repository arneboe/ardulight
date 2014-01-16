#include "mainwindow.h"
#include <QApplication>
#include "DirectX.h"
#include <QLabel>
#include <QPixmap>
#include <QPainter>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  AmbloneDirectX x;
  x.initialize(1);
  int* pdata = (int*)x.getScreenBitmap();
  QImage img(x.getWidth(), x.getHeight(),QImage::Format_ARGB32);
  for(int h = 0; h < x.getHeight(); ++h)
  {
    for(int w = 0; w < x.getWidth(); ++w, ++pdata)
    {
      img.setPixel(w, h, *pdata);
    }
  }

  QLabel myLabel;
  myLabel.setPixmap(QPixmap::fromImage(img));
  myLabel.show();

  return a.exec();
}
