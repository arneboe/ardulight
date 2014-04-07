#pragma once
#include <QtSerialPort/QtSerialPort>
/**
 * @brief Hardware interface to the ardulight.
 * Accessing the hardware will block until the command succeded.
 */
class Light
{
public:
  Light();
  ~Light();

  void setColor(const short pixelIndex, const unsigned char r, const unsigned char g,
                const unsigned char b);

  void show();

private:
  QSerialPort port;

};
