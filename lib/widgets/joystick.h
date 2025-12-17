#include "../animate.h"
#include "../draw.h"

struct {
  int enabled;

  double scale;       // overall joystick size scale
  gmColor background; // circle color
  gmColor border;     // border color

  struct {
    double scale; // hover scale
    gmColor border;
  } focussed;

  struct {
    double scale; // active scale (mouse down)
    gmColor border;
  } active;

  gmColor knob; // knob color
  gmColor knob_border;

  double border_width; // circle border width
} gmJoystick = {.enabled = 1,
                .scale = 1.0,
                .background = 0x3A2A3AE0,
                .border = 0x7F4F7FFF,
                .focussed = {.scale = 1.05, .border = 0xAA77AAFF},
                .active = {.scale = 0.95, .border = 0x7F4F7FFF},
                .knob = 0xAA77AAFF,
                .knob_border = 0x6F3F6FFF,
                .border_width = 0.01};

int gm_joystick_anim(double x, double y, double radius, gmPos *pos,
                     gmPos *vpos) {

  if (!gmJoystick.enabled)
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
    gm_anim_spring(&pos->x, 0, gm_dt(), 0.1);
    gm_anim_spring(&pos->y, 0, gm_dt(), 0.1);
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
  gm_anim_ease_out_quad(&vpos->x, pos->x, gm_dt(), 0.05);
  gm_anim_ease_out_quad(&vpos->y, pos->y, gm_dt(), 0.05);

  // Draw joystick circle (border + background)
  double draw_radius = radius * gmJoystick.scale;
  gm_draw_circle(x, y, draw_radius + gmJoystick.border_width,
                 gmJoystick.border);
  gm_draw_circle(x, y, draw_radius, gmJoystick.background);

  // Draw knob
  double knob_radius = draw_radius * 0.3; // 30% of radius
  gm_draw_circle(x + vpos->x * draw_radius, y + vpos->y * draw_radius,
                 knob_radius + gmJoystick.border_width, gmJoystick.knob_border);
  gm_draw_circle(
      x + vpos->x * draw_radius, y + vpos->y * draw_radius, knob_radius,
      nx == 0 && ny == 0 ? gmJoystick.knob & 0xEEEEFF44 : gmJoystick.knob);

  return hovered;
}

int gm_joystick(double x, double y, double radius, gmPos *pos) {
  return gm_joystick_anim(x, y, radius, pos, NULL);
}
