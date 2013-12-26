#include "mainwindow.h"
#include <QApplication>
#include "DirectX.h"
#include <QLabel>
#include <QPixmap>
#include <QPainter>
#include <QElapsedTimer>
#include <iostream>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  AmbloneDirectX x;
  x.initialize(1);
  QElapsedTimer timer;
  timer.start();
  unsigned int* p = (unsigned int*)x.getScreenBitmap();

  QImage img(100, 100,QImage::Format_ARGB32);
    for(int h = 0; h < 100; ++h)
    {
      for(int w = 0; w < 100; ++w, ++p)
      {
        img.setPixel(w, h, *p);

      }

    }

    QLabel myLabel;
    myLabel.setPixmap(QPixmap::fromImage(img));
    myLabel.show();


  return a.exec();
}
