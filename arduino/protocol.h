#pragma once
/**
 * Contains enums used in the hardware protocol
 */

/**
 * @brief The first byte of each message
 */
enum Command
{
  SET_COLOR, //sets all colors
  SET_BRIGHTNESS, //followed by one byte brightness
  RDY, //send by the arduino if it is ready to accept more data
  NUM_COMMANDS //Number of commands
};



