#pragma once
/**
 * Contains enums used in the hardware protocol
 */

/**
 * @brief The first byte of each message
 */
enum Command
{
  SET_COLOR, //parameter 0: short containing the pixel id, parameters 1,2 and 3 are RGB
  FLUSH, //displays the current colors
  NUM_COMMANDS //Number of commands
};


