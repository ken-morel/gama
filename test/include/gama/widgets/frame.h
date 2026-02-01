/**
 * @file widgets/frame.h
 * @brief Defines the theme and functionality for a frame widget.
 *
 * This file provides structures for customizing the appearance of frames
 * (panels) and a function to render them. Frames are typically used as
 * containers for other UI elements or for displaying information.
 */
#ifndef GAMA_WIDGETS_FRAME_H_INCLUDED
#define GAMA_WIDGETS_FRAME_H_INCLUDED

#include "../collision.h"
#include "../draw.h"

/**
 * @brief Structure defining the visual theme for a frame widget.
 */
typedef struct {
  int enabled;              /**< Whether the frame is interactive or displays focused state (1) or always displays inactive state (0). */

  gmColor background;       /**< Background color when normal. */
  gmColor border;           /**< Border color when normal. */
  double scale;             /**< Scale factor when normal. */

  struct {
    gmColor background;     /**< Background color when focused/hovered. */
    gmColor border;         /**< Border color when focused/hovered. */
    double scale;           /**< Scale factor when focused/hovered. */
  } focussed; /**< Theme properties when the frame is focused or hovered. */

  struct {
    gmColor background;     /**< Background color when inactive. */
    gmColor border;         /**< Border color when inactive. */
    double scale;           /**< Scale factor when inactive. */
  } inactive; /**< Theme properties when the frame is inactive. */

  double border_width;      /**< Width of the frame border. */
} gmwFrameTheme;

/**
 * @brief Global frame theme instance with default values.
 */
gmwFrameTheme gmwFrame = {.enabled = 1,

                          // Normal (semi-transparent overlay)
                          .background = 0x2A1E2AE0, // dark purple, ~88% alpha
                          .border = 0xAA77AAAF,
                          .scale = 1.0,

                          // Focused (menu panel highlighted)
                          .focussed =
                              {
                                  .background = 0x3A2A3AE8,
                                  .border = 0xBA87BAAF,
                                  .scale = 1.01,
                              },

                          // Inactive (background panels)
                          .inactive =
                              {
                                  .background = 0x201820C8,
                                  .border = 0x6F4F6FAF,
                                  .scale = 1.0,
                              },

                          .border_width = 0.015};

/**
 * @brief Creates and renders a frame widget (a bordered panel).
 *
 * This function draws a rectangular frame at the specified coordinates,
 * applying styling based on the global `gmwFrame` theme and its state
 * (hovered/focused or normal).
 *
 * @param x The x-coordinate of the frame's center.
 * @param y The y-coordinate of the frame's center.
 * @param width The width of the frame.
 * @param height The height of the frame.
 * @return 1 if the frame is currently hovered over, 0 otherwise.
 */
int gmw_frame(double x, double y, double width, double height) {
  int enabled = gmwFrame.enabled;
  int hovered = enabled && gm_mouse_in_rect(x, y, width, height);

  double scale = hovered ? gmwFrame.focussed.scale : gmwFrame.scale;

  gmColor bg = hovered ? gmwFrame.focussed.background : gmwFrame.background;

  gmColor border = hovered ? gmwFrame.focussed.border : gmwFrame.border;

  double sw = width * scale;
  double sh = height * scale;

  // Border (centered, square)
  gm_draw_rectangle(x, y, sw + gmwFrame.border_width * 2,
                    sh + gmwFrame.border_width * 2, border);

  // Background (semi-transparent)
  gm_draw_rectangle(x, y, sw, sh, bg);
  return hovered;
}

#endif // GAMA_WIDGETS_FRAME_H_INCLUDED
