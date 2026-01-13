#pragma once

#include "position.h"
#include <stdint.h>

/**
 * @brief Structure representing the current state of the mouse.
 *
 * This struct holds various properties related to mouse input, including
 * its position, movement, and button click states.
 */
struct _gmMouse {
  gmPos position;     /**< The current X,Y coordinates of the mouse. */
  gmPos lastPosition; /**< The X,Y coordinates of the mouse in the previous frame. */
  gmPos movement;     /**< The change in mouse position since the last frame. */

  uint8_t clicked;    /**< True (1) if the mouse button was just pressed in this frame, otherwise false (0). */
  uint8_t down;       /**< True (1) if the mouse button is currently held down, otherwise false (0). */
};

/**
 * @brief Global instance of the mouse state.
 *
 * This variable is updated automatically by the engine and provides
 * access to the current mouse input.
 * @example
 * if (gm_mouse.clicked) {
 *   // Mouse was just clicked
 * }
 * double mx = gm_mouse.position.x;
 */
struct _gmMouse gm_mouse = {
    .position = {0, 0},
    .lastPosition = {0, 0},
    .movement = {0, 0},
    .clicked = 0,
    .down = 0,
};
