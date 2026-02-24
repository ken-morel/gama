/**
 * @file widgets/joystick.h
 * @brief Defines the theme and functionality for a virtual joystick widget.
 *
 * This file provides structures for customizing the appearance of an on-screen
 * joystick and a function to render an interactive joystick that responds
 * to mouse input, outputting its normalized position.
 */
#ifndef GAMA_WIDGETS_JOYSTICK_H_INCLUDED
#define GAMA_WIDGETS_JOYSTICK_H_INCLUDED

#include "../animate.h"
#include "../draw.h"

/**
 * @brief Structure defining the visual theme for a joystick widget.
 */
typedef struct {
  int enabled; /**< Whether the joystick is enabled for interaction (1) or disabled (0). */

  double scale;       /**< Overall joystick size scale when normal. */
  gmColor background; /**< Background color of the joystick base circle when normal. */
  gmColor border;     /**< Border color of the joystick base circle when normal. */

  struct {
    double scale;   /**< Scale factor of the joystick when focused/hovered. */
    gmColor border; /**< Border color when focused/hovered. */
  } focussed; /**< Theme properties when the joystick is focused or hovered. */

  struct {
    double scale;   /**< Scale factor of the joystick when active (being dragged). */
    gmColor border; /**< Border color when active. */
  } active; /**< Theme properties when the joystick is active (mouse button down on it). */

  gmColor knob;        /**< Color of the joystick knob. */
  gmColor knob_border; /**< Border color of the joystick knob. */

  double border_width; /**< Width of the joystick base circle border. */
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
 *
 * This function draws a virtual joystick on screen, tracks mouse interaction,
 * and outputs its normalized logical position. The visual knob position is
 * animated smoothly.
 *
 * @param x The x-coordinate of the joystick's center.
 * @param y The y-coordinate of the joystick's center.
 * @param radius The radius of the joystick base circle.
 * @param pos Pointer to a `gmPos` structure to store the joystick's logical
 *        position (normalized to -1.0 to 1.0 in X and Y).
 * @param vpos Pointer to a `gmPos` structure for the animated visual knob
 *        position. If NULL, `pos` is used for both logical and visual position.
 * @return 1 if the joystick is currently hovered over, 0 otherwise.
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
 * mouse (non-animated version).
 *
 * This is a simplified wrapper around `gm_joystick_anim` that does not use
 * a separate visual position for animation, making the knob snap to position.
 *
 * @param x The x-coordinate of the joystick's center.
 * @param y The y-coordinate of the joystick's center.
 * @param radius The radius of the joystick base.
 * @param pos Pointer to a `gmPos` structure to store the joystick's logical
 *        position (normalized to -1.0 to 1.0 in X and Y).
 * @return 1 if the joystick is currently hovered over, 0 otherwise.
 */
int gmw_joystick(double x, double y, double radius, gmPos *pos) {
  return gm_joystick_anim(x, y, radius, pos, NULL);
}

#endif // GAMA_WIDGETS_JOYSTICK_H_INCLUDED
