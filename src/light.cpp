#include "light.h"
#include "global.h"
#include <QObject>
#include "../arduino/protocol.h"
#include <QThread>
#include <QDateTime>

Light::Light() : lastShowTimestamp(0), port(NULL)
{
  const qint32 baud = Global::getInstance().getSettings().value("Hardware/baudRate", 115200).toInt();
  const QString portName = Global::getInstance().getSettings().value("Hardware/port", "COM3").toString();
  port.setPortName(portName);

  if(!port.open(QIODevice::ReadWrite))
  {
    qFatal("Unable to open serial port");
    exit(1); //FIXME this is bad style :D
  }
  if(!port.setBaudRate(baud))
  {
    qFatal("Unable to set serial port baud rate");
    exit(1); //FIXME baaad
  }
  if(!port.setFlowControl(QSerialPort::NoFlowControl))
  {
    qFatal("Unable to set flow control");
    exit(1); //FIXME baaad
  }
}

void Light::setColor(const short pixelIndex, const unsigned char r, const unsigned char g,
                     const unsigned char b)
{
  buffer[pixelIndex].r = r;
  buffer[pixelIndex].g = g;
  buffer[pixelIndex].b = b;
}

void Light::setAllColors(const unsigned char r, const unsigned char g, const unsigned char b)
{
  for(Color& c : buffer)
  {
    c.r = r;
    c.g = g;
    c.b = b;
  }
}

void Light::sendColors()
{
  if(port.isOpen())
  {
    QByteArray data;
    data.append(SET_COLOR);
    for(const Color& c : buffer)
    {
      data.append(c.r);
      data.append(c.g);
      data.append(c.b);
    }
    send(data);
  }
  else
  {
    qWarning("Unable to execute command, serial port is closed.");
  }
}

void Light::send(const QByteArray& data)
{
  if(waitForReady())
  {
    port.write(data);
    if(!port.waitForBytesWritten(1000))
    {
     qWarning("Failed to send data to the arduino (write timeout after 1 second).");
    }
  }
  else
  {
    qWarning("send failed. Device not ready");
  }
}

bool Light::waitForReady()
{
  char data[1];
  data[0] = -1;
  if(port.waitForReadyRead(10000))
  {
    const int numRead = port.read(&data[0], 1);
    if(numRead == 1)
    {
      return data[0] == (char)RDY;
    }
    else
    {
      qWarning("Read error, read %d bytes", numRead);
    }
  }
  else
  {
    qWarning("Read timeout");
    return false;
  }
  return false;
}

Light::~Light()
{
  if(port.isOpen())
  {
    port.close();
  }
}
