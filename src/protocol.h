#pragma once
/**
 * Contains enums used in the hardware protocol
 */

/**
 * @brief The first byte of each message
 */
enum Command
{
  SET_LED_COUNT //first parameter is an unsigned short indicating the number of leds
};
