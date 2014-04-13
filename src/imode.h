#pragma once

class Light;
/**
 * This class represents a light mode.
 * There are different light modes which are sub classes of this class.
 *
 * There can only be one Mode in control of the light at any given time.
 * If a Mode is in control it can do whatever it want to the light.
 */
class IMode
{
public:
  virtual void startMode(Light* light) = 0; //starts the Mode, may not block.
  virtual void stopMode() = 0;//stops the Mode, may block until operation has finished.
  virtual QString getName() const = 0;
};

