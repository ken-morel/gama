#include "../collision.h"
#include "../draw.h"

struct {
  int enabled;

  gmColor background;
  gmColor border;
  double scale;

  struct {
    gmColor background;
    gmColor border;
    double scale;
  } focussed;

  struct {
    gmColor background;
    gmColor border;
    double scale;
  } inactive;

  double border_width;
} gmFrame = {.enabled = 1,

             // Normal (semi-transparent overlay)
             .background = 0x2A1E2AE0, // dark purple, ~88% alpha
             .border = 0xAA77AAAF,
             .scale = 1.0,

             // Focused (menu panel highlighted)
             .focussed =
                 {
                     .background = 0x3A2A3AE8,
                     .border = 0xBA87BAAF,
                     .scale = 1.02,
                 },

             // Inactive (background panels)
             .inactive =
                 {
                     .background = 0x201820C8,
                     .border = 0x6F4F6FAF,
                     .scale = 1.0,
                 },

             .border_width = 0.015};

int gm_frame(double x, double y, double width, double height) {
  int enabled = gmFrame.enabled;
  int hovered = enabled && gm_mouse_in_rect(x, y, width, height);

  double scale = hovered ? gmFrame.focussed.scale : gmFrame.scale;

  gmColor bg = hovered ? gmFrame.focussed.background : gmFrame.background;

  gmColor border = hovered ? gmFrame.focussed.border : gmFrame.border;

  double sw = width * scale;
  double sh = height * scale;

  // Border (centered, square)
  gm_draw_rectangle(x, y, sw + gmFrame.border_width * 2,
                    sh + gmFrame.border_width * 2, border);

  // Background (semi-transparent)
  gm_draw_rectangle(x, y, sw, sh, bg);
  return hovered;
}
