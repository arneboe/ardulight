#include "Light.h"
#include "Global.h"
#include <QObject>
#include "../arduino/protocol.h"
#include <QMutexLocker>
#include <iostream>

Light::Light() : lastShowTimestamp(0), port(this)
{
  const qint32 baud = Global::getInstance().getSettings().value("Hardware/BaudRate", 115200).toInt();
  const QString portName = Global::getInstance().getSettings().value("Hardware/Port", "COM3").toString();
  port.setPortName(portName);

  if(!port.open(QIODevice::ReadWrite))
  {
    qFatal("Unable to open serial port");
    exit(1); //FIXME this is bad style :D
  }
  if(!port.setParity(QSerialPort::NoParity) ||
     !port.setStopBits(QSerialPort::OneStop) ||
     !port.setDataBits(QSerialPort::Data8) ||
     !port.setFlowControl(QSerialPort::NoFlowControl ) ||
     !port.setBaudRate(baud) ||
     !port.setDataTerminalReady(true))
  {
     qFatal("Unable to configure serial port");
     exit(1); //FIXME baaad
  }
  if(port.error() != QSerialPort::NoError)
  {
    qFatal("some error occurred!");
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

void Light::moveToThread(QThread *targetThread)
{
  QObject::moveToThread(targetThread);
}

QThread *Light::thread() const
{
  return QObject::thread();
}

void Light::send(const QByteArray& data)
{
  QMutexLocker ml(&sendMutex);
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

void Light::setBrightness(const int brightness)
{
  Q_ASSERT(brightness >= 0);
  Q_ASSERT(brightness <= 255);
  QByteArray data;
  data.append(SET_BRIGHTNESS);
  data.append((unsigned char)brightness);
  send(data);
}

