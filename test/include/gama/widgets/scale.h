#pragma once

#include "../animate.h"
#include "../collision.h"
#include "../draw.h"

/**
 * @brief Structure defining the visual theme for a scale (slider) widget.
 */
typedef struct {
  int enabled; /**< Whether the scale is enabled */

  double scale; /**< Overall widget scale */

  gmColor background; /**< Track background color */
  gmColor border;     /**< Track border color */

  struct {
    double scale;   /**< Scale factor when focused/hovered */
    gmColor border; /**< Border color when focused/hovered */
  } focussed;

  struct {
    double scale;   /**< Scale factor when active pressed */
    gmColor border; /**< Border color when active pressed */
  } active;

  gmColor knob;        /**< Knob color */
  gmColor knob_border; /**< Knob border color */

  double border_width; /**< Track border thickness */
  double step; /**< Step size for discrete values (0 = allow any value) */
} gmwScaleTheme;

/**
 * @brief Global scale theme instance with default values.
 */
gmwScaleTheme gmwScale = {.enabled = 1,
                          .scale = 1.0,

                          .background = 0x3A2A3AE0,
                          .border = 0x7F4F7FFF,

                          .focussed = {.scale = 1.03, .border = 0xAA77AAFF},
                          .active = {.scale = 0.97, .border = 0x7F4F7FFF},

                          .knob = 0xAA77AAFF,
                          .knob_border = 0x6F3F6FFF,

                          .border_width = 0.01,
                          .step = 0.0};

/**
 * @brief Creates and renders an animated scale (slider) widget that can be
 * manipulated with the mouse.
 * @param x The x-coordinate of the scale's center.
 * @param y The y-coordinate of the scale's center.
 * @param width The width of the scale track.
 * @param height The height of the scale track.
 * @param value Pointer to a double to store the current scale value (0.0
 * to 1.0).
 * @param anim Pointer to a double for animated visual position (can be NULL to
 * use value).
 * @return 1 if the scale is currently being actively manipulated (mouse down),
 * 0 otherwise.
 */
int gmw_scale_anim(double x, double y, double width, double height,
                   double *value, double *anim) {

  if (value == NULL)
    return 0;
  if (*value < 0)
    *value = 0;
  if (*value > 1)
    *value = 1;
  if (anim != NULL && (*anim > 1 || *anim < 0))
    *anim = *value;

  int enabled = gmwScale.enabled;

  // Hover test uses full widget size (logical width/height)
  int hovered = enabled && gm_mouse_in_rect(x, y, width, height);
  int active = enabled && gm_mouse.down && hovered;

  if (anim == NULL)
    anim = value;
  else
    gm_anim_ease_out_quad(anim, (double)(*value), 0.05);

  // Visual scale / border
  double scale = 1.0;
  gmColor border = gmwScale.border;
  if (active) {
    scale = gmwScale.active.scale;
    border = gmwScale.active.border;
  } else if (hovered) {
    scale = gmwScale.focussed.scale;
    border = gmwScale.focussed.border;
  } else {
    scale = gmwScale.scale;
  }

  // Draw sizes (scaled visually)
  double sw = width * scale;
  double sh = height * scale;

  // Draw track border
  gm_draw_rectangle(x, y, sw, sh, border);

  // Track inner size (for knob movement)
  double track_w = sw - gmwScale.border_width * 2;
  double track_h = sh - gmwScale.border_width * 2;

  // Draw track
  gm_draw_rectangle(x, y, track_w, track_h, gmwScale.background);

  // Orientation
  int horizontal = (width >= height);

  // Handle dragging along logical track (not scaled)
  if (active) {
    if (horizontal) {
      double left = x - width * 0.5 + gmwScale.border_width;
      double rel =
          (gm_mouse.position.x - left) / (width - gmwScale.border_width * 2);
      if (rel < 0.0)
        rel = 0.0;
      if (rel > 1.0)
        rel = 1.0;
      if (gmwScale.step > 0.0)
        rel = ((int)(rel / gmwScale.step + 0.5)) * gmwScale.step;
      *value = rel;
    } else {
      double bottom = y - height * 0.5 + gmwScale.border_width;
      double rel =
          (gm_mouse.position.y - bottom) / (height - gmwScale.border_width * 2);
      if (rel < 0.0)
        rel = 0.0;
      if (rel > 1.0)
        rel = 1.0;
      if (gmwScale.step > 0.0)
        rel = ((int)(rel / gmwScale.step + 0.5)) * gmwScale.step;
      *value = rel;
    }
  }

  // Draw knob
  double knob_size = (horizontal ? sh : sw) * 1.5;
  double knob_x, knob_y;

  if (horizontal) {
    double left = x - sw * 0.5 + knob_size * 0.5;
    double right = x + sw * 0.5 - knob_size * 0.5;
    knob_x = left + (*anim) * (right - left);
    knob_y = y;
  } else {
    double bottom = y - sh * 0.5 + knob_size * 0.5;
    double top = y + sh * 0.5 - knob_size * 0.5;
    knob_y = bottom + (*anim) * (top - bottom);
    knob_x = x;
  }

  // Draw knob border
  gm_draw_rectangle(knob_x, knob_y, knob_size + gmwScale.border_width * 2,
                    knob_size + gmwScale.border_width * 2,
                    gmwScale.knob_border);

  // Draw knob
  gm_draw_rectangle(knob_x, knob_y, knob_size, knob_size, gmwScale.knob);

  return active;
}

/**
 * @brief Creates and renders a scale (slider) widget that can be manipulated
 * with the mouse.
 * @param x The x-coordinate of the scale's center.
 * @param y The y-coordinate of the scale's center.
 * @param width The width of the scale track.
 * @param height The height of the scale track.
 * @param value Pointer to a double to store the current scale value (0.0
 * to 1.0).
 * @return 1 if the scale is currently being actively manipulated (mouse down),
 * 0 otherwise.
 */
static inline int gmw_scale(double x, double y, double width, double height,
                            double *value) {
  return gmw_scale_anim(x, y, width, height, value, NULL);
}
