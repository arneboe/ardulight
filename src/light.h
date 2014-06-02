#pragma once
#include <QtSerialPort/QtSerialPort>
#include <QObject>
#include <QMutex>
#include "ILight.h"
/**
 * @brief Hardware interface to the ardulight.
 * Accessing the hardware will block until the command succeded.
 */
class Light : public QObject, ILight
{
  Q_OBJECT;
public:
  Light();
  ~Light();

  void setColor(const short pixelIndex, const unsigned char r, const unsigned char g,
                const unsigned char b);

  void setAllColors(const unsigned char r, const unsigned char g, const unsigned char b);

  /**Sends all colors to the arduino and shows them.
     Is thread safe*/
  void sendColors();

  void moveToThread(QThread * targetThread);
  QThread *thread() const;

  /**Is thread safe */
public slots:
  void setBrightness(const int brightness);

private:
  /**Blocks till the arduino is ready*/
  bool waitForReady();

  void send(const QByteArray& data);

  struct Color
  {
    unsigned char r;
    unsigned char g;
    unsigned char b;
  };

  qint64 lastShowTimestamp; /**<timestamp of the last show() call */
  QSerialPort port;
  Color buffer[60]; //FIXME led number should not be hard code
  QMutex sendMutex;
};
