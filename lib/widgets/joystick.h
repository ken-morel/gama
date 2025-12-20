#pragma once

#include "../animate.h"
#include "../draw.h"

/**
 * @brief Structure defining the visual theme for a joystick widget.
 */
typedef struct {
  int enabled; /**< Whether the joystick is enabled */

  double scale;       /**< Overall joystick size scale */
  gmColor background; /**< Circle background color */
  gmColor border;     /**< Border color */

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

  double border_width; /**< Width of the circle border */
} gmwJoystickTheme;

/**
 * @brief Global joystick theme instance with default values.
 */
gmwJoystickTheme gmwJoystick = {
    .enabled = 1,
    .scale = 1.0,
    .background = 0x3A2A3AE0,
    .border = 0x7F4F7FFF,
    .focussed = {.scale = 1.05, .border = 0xAA77AAFF},
    .active = {.scale = 0.95, .border = 0x7F4F7FFF},
    .knob = 0xAA77AAFF,
    .knob_border = 0x6F3F6FFF,
    .border_width = 0.01};

/**
 * @brief Creates and renders an animated joystick widget that can be
 * manipulated with the mouse.
 * @param x The x-coordinate of the joystick's center.
 * @param y The y-coordinate of the joystick's center.
 * @param radius The radius of the joystick base.
 * @param pos Pointer to a gmPos structure to store the joystick's logical
 * position (-1 to 1).
 * @param vpos Pointer to a gmPos structure for animated visual position (can be
 * NULL to use pos).
 * @return 1 if the joystick is currently hovered, 0 otherwise.
 */
int gm_joystick_anim(double x, double y, double radius, gmPos *pos,
                     gmPos *vpos) {

  if (!gmwJoystick.enabled)
    return 0;
  if (vpos == NULL)
    vpos = pos;

  // Mouse vector relative to center
  double dx = gm_mouse.position.x - x;
  double dy = gm_mouse.position.y - y;

  double dist = sqrt(dx * dx + dy * dy);

  int hovered = dist <= radius;

  int active = hovered && gm_mouse.down;

  if (!gm_mouse.down) {
    gm_anim_ease_out_cubic(&pos->x, 0, 0.1);
    gm_anim_ease_out_cubic(&pos->y, 0, 0.1);
  }

  // Logical joystick position (-1..1)
  double nx = 0.0, ny = 0.0;

  if (active) {
    if (dist > radius) {
      dx = dx / dist * radius;
      dy = dy / dist * radius;
    }
    nx = dx / radius;
    ny = dy / radius;

    pos->x = nx;
    pos->y = ny;
  }

  // Animate knob smoothly to target pos->x/pos->y
  gm_anim_ease_out_quad(&vpos->x, pos->x, 0.05);
  gm_anim_ease_out_quad(&vpos->y, pos->y, 0.05);

  // Draw joystick circle (border + background)
  double draw_radius = radius * gmwJoystick.scale;
  gm_draw_circle(x, y, draw_radius + gmwJoystick.border_width,
                 gmwJoystick.border);
  gm_draw_circle(x, y, draw_radius, gmwJoystick.background);

  // Draw knob
  double knob_radius = draw_radius * 0.3; // 30% of radius
  gm_draw_circle(x + vpos->x * draw_radius, y + vpos->y * draw_radius,
                 knob_radius + gmwJoystick.border_width,
                 gmwJoystick.knob_border);
  gm_draw_circle(
      x + vpos->x * draw_radius, y + vpos->y * draw_radius, knob_radius,
      nx == 0 && ny == 0 ? gmwJoystick.knob & 0xEEEEFF44 : gmwJoystick.knob);

  return hovered;
}

/**
 * @brief Creates and renders a joystick widget that can be manipulated with the
 * mouse.
 * @param x The x-coordinate of the joystick's center.
 * @param y The y-coordinate of the joystick's center.
 * @param radius The radius of the joystick base.
 * @param pos Pointer to a gmPos structure to store the joystick's logical
 * position (-1 to 1).
 * @return 1 if the joystick is currently hovered, 0 otherwise.
 */
int gmw_joystick(double x, double y, double radius, gmPos *pos) {
  return gm_joystick_anim(x, y, radius, pos, NULL);
}
