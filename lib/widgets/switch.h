#include "../animate.h"
#include "../collision.h"
#include "../draw.h"

struct {
  int enabled;

  double scale;

  struct {
    gmColor background;
    gmColor border;
    gmColor knob;
    gmColor knob_border;
  } off;

  struct {
    gmColor background;
    gmColor border;
    gmColor knob;
    gmColor knob_border;
  } on;

  struct {
    double scale;
    gmColor border;
  } focussed;

  struct {
    double scale;
    gmColor border;
  } active;

  double border_width;
} gmSwitch = {.enabled = 1,

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

int gm_switch_anim(double x, double y, double width, double height, int *value,
                   double *anim) {

  int enabled = gmSwitch.enabled;

  int hovered = enabled && gm_mouse_in_rect(x, y, width, height);
  int clicked = enabled && gm_mouse.pressed && hovered;

  if (clicked) {
    *value = !(*value);
  }
  double knob_pos;
  if (anim != NULL) {
    gm_anim_ease_out_quad(anim, (double)(*value), gm_dt(), 0.05);
    knob_pos = *anim;

  } else {
    knob_pos = (double)(*value);
  }
  int on = (*value != 0);

  double scale = gm_mouse.down && hovered ? gmSwitch.active.scale
                 : hovered                ? gmSwitch.focussed.scale
                                          : 1.0;

  gmColor bg = on ? gmSwitch.on.background : gmSwitch.off.background;
  gmColor border = on ? gmSwitch.on.border : gmSwitch.off.border;
  gmColor knob = on ? gmSwitch.on.knob : gmSwitch.off.knob;
  gmColor knob_b = on ? gmSwitch.on.knob_border : gmSwitch.off.knob_border;

  if (hovered)
    border = gmSwitch.focussed.border;
  if (gm_mouse.down && hovered)
    border = gmSwitch.active.border;

  double sw = width * scale;
  double sh = height * scale;

  // Outer border
  gm_draw_rectangle(x, y, sw + gmSwitch.border_width * 2,
                    sh + gmSwitch.border_width * 2, border);

  // Background
  gm_draw_rectangle(x, y, sw, sh, bg);

  // Knob (square)
  double knob_size = sh * 0.8;
  double knob_x_offset =
      (knob_pos * 2 - 1) * (sw * 0.5 - knob_size * 0.5 - gmSwitch.border_width);
  // Knob border
  gm_draw_rectangle(x + knob_x_offset, y, knob_size + gmSwitch.border_width * 2,
                    knob_size + gmSwitch.border_width * 2, knob_b);

  // Knob
  gm_draw_rectangle(x + knob_x_offset, y, knob_size, knob_size, knob);

  return hovered;
}
int gm_switch(double x, double y, double width, double height, int *value) {
  return gm_switch_anim(x, y, width, height, value, NULL);
}
