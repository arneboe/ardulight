#pragma once

/**
 * @brief Hardware interface to the ardulight.
 * Accessing the hardware will block until the command succeded.
 *
 * @note Controllers may move the light to other threads as long as they
 *       are active. But as soon as a controller goes inactive it should
 *       move the light back to the original thread.
 */
class ILight
{
public:

  virtual void setColor(const short pixelIndex, const unsigned char r,
                        const unsigned char g, const unsigned char b) = 0;

  virtual void setAllColors(const unsigned char r, const unsigned char g,
                            const unsigned char b) = 0;

  /**Sends all colors to the arduino and shows them.
     Is thread safe*/
  virtual void sendColors() = 0;

  //The following methods are part of QObject's interface.
  //They are reproduced here because the interface needs to be pure virtual
  //for the qt plugin system to work
  //Any ILight implementation should simply forward these calls to
  //their QObject equivalents
  virtual void moveToThread(QThread * targetThread) = 0;
  virtual QThread* thread() const = 0;

  /**Is thread safe */
public slots:
  virtual void setBrightness(const int brightness) = 0;
};
