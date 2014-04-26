#pragma once
#include <QSettings>

/**
 * @brief Contains some static stuff that needs to be available globally
 *        i.e. settings.
 */
class Global
{
  //see http://stackoverflow.com/questions/1008019/c-singleton-design-pattern/1008289#1008289
public:
    static Global& getInstance()
    {
        static Global instance;
        return instance;
    }

    QSettings& getSettings();

private:
    //only declared to avoid accidental copies
    Global();
    QSettings settings;
};
