#include "light.h"
#include "global.h"
#include <QObject>
#include "../arduino/protocol.h"

Light::Light() : port(NULL)
{
  const qint32 baud = Global::getInstance().getSettings().value("Hardware/baudRate", 115200).toInt();
  const QString portName = Global::getInstance().getSettings().value("Hardware/port", "COM3").toString();
  port.setPortName(portName);
  if(!port.open(QIODevice::WriteOnly))
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
  if(port.isOpen())
  {
    QByteArray data;
    data.append(SET_COLOR);
    data.append((const char*)&pixelIndex, sizeof(short));
    data.append(r);
    data.append(g);
    data.append(b);
    Q_ASSERT(data.size() == 6);
    port.write(data);
    if(!port.waitForBytesWritten(1000))
    {
      qWarning("Failed to send data to the arduino (write timeout after 1 second).");
    }
  }
  else
  {
    qWarning("Unable to execute command, serial port is closed.");
  }
}


void Light::show()
{
  if(port.isOpen())
  {
    QByteArray data(1, FLUSH);
    port.write(data);
    if(!port.waitForBytesWritten(1000))
    {
      qWarning("Failed to send data to the arduino (write timeout after 1 second).");
    }
  }
  else
  {
    qWarning("Unable to execute command, serial port is closed.");
  }
}

Light::~Light()
{
  if(port.isOpen())
  {
    port.close();
  }
}
