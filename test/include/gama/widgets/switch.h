/**
 * @file widgets/switch.h
 * @brief Defines the theme and functionality for a toggle switch widget.
 *
 * This file provides structures for customizing the appearance of an on-screen
 * toggle switch and a function to render an interactive switch that allows
 * users to toggle a boolean state.
 */
#ifndef GAMA_WIDGETS_SWITCH_H_INCLUDED
#define GAMA_WIDGETS_SWITCH_H_INCLUDED

#include "../animate.h"
#include "../collision.h"
#include "../draw.h"

/**
 * @brief Structure defining the visual theme for a switch widget.
 */
typedef struct {
  int enabled; /**< Whether the switch is enabled for interaction (1) or disabled (0). */

  double scale; /**< Default overall widget scale when normal. */

  struct {
    gmColor background;  /**< Background color when the switch is off. */
    gmColor border;      /**< Border color when the switch is off. */
    gmColor knob;        /**< Knob color when the switch is off. */
    gmColor knob_border; /**< Knob border color when the switch is off. */
  } off; /**< Theme properties for the 'off' state of the switch. */

  struct {
    gmColor background;  /**< Background color when the switch is on. */
    gmColor border;      /**< Border color when the switch is on. */
    gmColor knob;        /**< Knob color when the switch is on. */
    gmColor knob_border; /**< Knob border color when the switch is on. */
  } on; /**< Theme properties for the 'on' state of the switch. */

  struct {
    double scale;   /**< Scale factor when focused/hovered. */
    gmColor border; /**< Border color when focused/hovered. */
  } focussed; /**< Theme properties when the switch is focused or hovered. */

  struct {
    double scale;   /**< Scale factor when active (being clicked). */
    gmColor border; /**< Border color when active. */
  } active; /**< Theme properties when the switch is active (mouse button down on it). */

  double border_width; /**< Width of the switch's outer border. */
} gmwSwitchTheme;

/**
 * @brief Global switch theme instance with default values.
 */
gmwSwitchTheme gmwSwitch = {.enabled = 1,

                            // OFF state (muted)
                            .off =
                                {
                                    .background = 0x2A1E2AE0,
                                    .border = 0x5F4F5FFF,
                                    .knob = 0x8A7A8AFF,
                                    .knob_border = 0x5F4F5FFF,
                                },

                            // ON state (clear & branded)
                            .on =
                                {
                                    .background = 0x4A2A4AE8,
                                    .border = 0xAA77AAFF,
                                    .knob = 0xEED6EEFF,
                                    .knob_border = 0xAA77AAFF,
                                },

                            // Hover
                            .focussed =
                                {
                                    .scale = 1.04,
                                    .border = 0xBA87BAFF,
                                },

                            // Pressed
                            .active =
                                {
                                    .scale = 0.97,
                                    .border = 0x7F4F7FFF,
                                },

                            .border_width = 0.01};

/**
 * @brief Creates and renders an animated switch widget that toggles on click.
 *
 * This function draws a toggle switch, tracks mouse interaction to update its
 * state, and animates the knob's position smoothly between 'off' (0) and 'on' (1).
 *
 * @param x The x-coordinate of the switch's center.
 * @param y The y-coordinate of the switch's center.
 * @param width The width of the switch.
 * @param height The height of the switch.
 * @param value Pointer to an integer to store the switch state (0 for off, 1 for on).
 *        This value is toggled by user clicks.
 * @param anim Pointer to a double for the animated visual position of the knob.
 *        If NULL, `value` is used for the knob's position (non-animated).
 * @return 1 if the switch was clicked (toggled) in the current frame, 0 otherwise.
 */
int gmw_switch_anim(double x, double y, double width, double height, int *value,
                    double *anim) {

  int enabled = gmwSwitch.enabled;

  int hovered = enabled && gm_mouse_in_rect(x, y, width, height);
  int clicked = enabled && gm_mouse.clicked && hovered;

  if (clicked) {
    *value = !(*value);
  }
  double knob_pos;
  if (anim != NULL) {
    if (*anim > 1)
      *anim = 1;
    else if (*anim < 0)
      *anim = 0;
    gm_anim_ease_out_quad(anim, (double)(*value), 0.05);
    knob_pos = *anim;
  } else {
    knob_pos = (double)(*value);
  }
  int on = *value != 0;

  double scale = gm_mouse.down && hovered ? gmwSwitch.active.scale
                 : hovered                ? gmwSwitch.focussed.scale
                                          : 1.0;

  gmColor bg = on ? gmwSwitch.on.background : gmwSwitch.off.background;
  gmColor border = on ? gmwSwitch.on.border : gmwSwitch.off.border;
  gmColor knob = on ? gmwSwitch.on.knob : gmwSwitch.off.knob;
  gmColor knob_b = on ? gmwSwitch.on.knob_border : gmwSwitch.off.knob_border;

  if (hovered)
    border = gmwSwitch.focussed.border;
  if (gm_mouse.down && hovered)
    border = gmwSwitch.active.border;

  double sw = width * scale;
  double sh = height * scale;

  // Outer border
  gm_draw_rectangle(x, y, sw + gmwSwitch.border_width * 2,
                    sh + gmwSwitch.border_width * 2, border);

  // Background
  gm_draw_rectangle(x, y, sw, sh, bg);

  // Knob (square)
  double knob_size = sh * 0.8;
  double knob_x_offset = (knob_pos * 2 - 1) *
                         (sw * 0.5 - knob_size * 0.5 - gmwSwitch.border_width);
  // Knob border
  gm_draw_rectangle(x + knob_x_offset, y,
                    knob_size + gmwSwitch.border_width * 2,
                    knob_size + gmwSwitch.border_width * 2, knob_b);

  // Knob
  gm_draw_rectangle(x + knob_x_offset, y, knob_size, knob_size, knob);

  return clicked;
}

/**
 * @brief Creates and renders a switch widget that toggles on click (non-animated version).
 *
 * This is a simplified wrapper around `gmw_switch_anim` that does not use
 * a separate animated visual position, causing the knob to snap to the
 * current value.
 *
 * @param x The x-coordinate of the switch's center.
 * @param y The y-coordinate of the switch's center.
 * @param width The width of the switch.
 * @param height The height of the switch.
 * @param value Pointer to an integer to store the switch state (0=off, 1=on).
 * @return 1 if the switch was clicked (toggled) in the current frame, 0 otherwise.
 */
int gmw_switch(double x, double y, double width, double height, int *value) {
  return gmw_switch_anim(x, y, width, height, value, NULL);
}

#endif // GAMA_WIDGETS_SWITCH_H_INCLUDED
