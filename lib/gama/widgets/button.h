/**
 * @file widgets/button.h
 * @brief Defines the theme and functionality for a button widget.
 *
 * This file provides structures for customizing the appearance of buttons
 * and a function to render an interactive button that responds to mouse input.
 */
#ifndef GM_BUTTON_H_INCLUDED
#define GM_BUTTON_H_INCLUDED
#include "../collision.h"
#include "../draw.h"

/**
 * @brief Structure defining the visual theme for a button widget.
 */
typedef struct {
  int enabled; /**< Whether the button is interactive (1) or disabled (0). */

  double scale;       /**< Default scale of the button. */
  gmColor background; /**< Background color when normal. */
  gmColor border;     /**< Border color when normal. */
  gmColor text;       /**< Text color when normal. */

  struct {
    gmColor background; /**< Background color when focused/hovered. */
    gmColor border;     /**< Border color when focused/hovered. */
    double scale;       /**< Scale when focused/hovered. */
    gmColor text;       /**< Text color when focused/hovered. */
  } focussed; /**< Theme properties when the button is focused or hovered. */

  struct {
    gmColor background; /**< Background color when active/pressed. */
    gmColor border;     /**< Border color when active/pressed. */
    double scale;       /**< Scale when active/pressed. */
    gmColor text;       /**< Text color when active/pressed. */
  } active; /**< Theme properties when the button is active (mouse button down
               on it). */

  struct {
    gmColor background; /**< Background color when disabled. */
    gmColor border;     /**< Border color when disabled. */
    gmColor text;       /**< Text color when disabled. */
  } disabled;           /**< Theme properties when the button is disabled. */

  double border_thickness; /**< Thickness of the button border. */
  const char *font;        /**< Font used for button text. */
} gmwButtonTheme;

/**
 * @brief Global button theme instance with default values.
 */
gmwButtonTheme gmwButton = {.enabled = 1,

                            // Normal
                            .background = 0xAA77AAFF,
                            .border = 0x7F4F7FFF,
                            .text = GM_WHITE,
                            .scale = 1.0,

                            // Focus / Hover
                            .focussed =
                                {
                                    .background = 0xBA87BAFF,
                                    .border = 0x9F6F9FFF,
                                    .scale = 1.04,
                                    .text = GM_WHITE,
                                },

                            // Active / Pressed
                            .active =
                                {
                                    .background = 0x9B5F9BFF,
                                    .border = 0x6F3F6FFF,
                                    .scale = 0.97,
                                    .text = GM_WHITE,
                                },

                            // Disabled
                            .disabled =
                                {
                                    .background = 0x9A8F9AFF,
                                    .border = 0x6E646EFF,
                                    .text = 0xDDDDDDFF,
                                },

                            .border_thickness = 0.01,
                            .font = ""};

/**
 * @brief Creates and renders an interactive button widget.
 *
 * This function draws a button at the specified coordinates and handles
 * its interactive states (normal, hovered, active, disabled) based on
 * mouse input and the global `gmwButton` theme.
 *
 * @param x The x-coordinate of the button's center.
 * @param y The y-coordinate of the button's center.
 * @param width The width of the button.
 * @param height The height of the button.
 * @param text The text to display on the button (can be NULL).
 * @param fontsize The size of the text font.
 * @return 1 if the button is currently hovered over, 0 otherwise.
 */
int gmw_button(double x, double y, double width, double height,
               const char *text, double fontsize) {

  int enabled = gmwButton.enabled;

  int hovered = enabled && gm_mouse_in_rect(x, y, width, height);
  int clicked = enabled && gm_mouse.down && hovered;

  double scale = !enabled  ? gmwButton.scale
                 : clicked ? gmwButton.active.scale
                 : hovered ? gmwButton.focussed.scale
                           : gmwButton.scale;

  gmColor bg = !enabled  ? gmwButton.disabled.background
               : clicked ? gmwButton.active.background
               : hovered ? gmwButton.focussed.background
                         : gmwButton.background;

  gmColor border = !enabled  ? gmwButton.disabled.border
                   : clicked ? gmwButton.active.border
                   : hovered ? gmwButton.focussed.border
                             : gmwButton.border;

  gmColor fg = !enabled  ? gmwButton.disabled.text
               : clicked ? gmwButton.active.text
               : hovered ? gmwButton.focussed.text
                         : gmwButton.text;

  // Scale expands symmetrically from the center
  double sw = width * scale;
  double sh = height * scale;

  // Border (square, centered)
  gm_draw_rectangle(x, y, sw + gmwButton.border_thickness * 2,
                    sh + gmwButton.border_thickness * 2, border);

  // Background (centered)
  gm_draw_rectangle(x, y, sw, sh, bg);

  // Text (already center-based)
  if (text) {
    gm_draw_text(x, y - fontsize / 10, text, gmwButton.font, fontsize * scale,
                 fg);
  }

  return hovered;
}

#endif // GM_BUTTON_H_INCLUDED
