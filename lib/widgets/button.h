#include "../collision.h"
#include "../draw.h"

struct {
  int enabled;

  double scale;
  gmColor background;
  gmColor border;
  gmColor text;

  struct {
    gmColor background;
    gmColor border;
    double scale;
    gmColor text;
  } focussed;

  struct {
    gmColor background;
    gmColor border;
    double scale;
    gmColor text;
  } active;

  struct {
    gmColor background;
    gmColor border;
    gmColor text;
  } disabled;

  double border_thickness;
  const char *font;
} gmButton = {.enabled = 1,

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
              .font = "default-ui"};

int gm_button(double x, double y, double width, double height, const char *text,
              double fontsize) {

  int enabled = gmButton.enabled;

  int hovered = enabled && gm_mouse_in_rect(x, y, width, height);
  int clicked = enabled && gm_mouse.down && hovered;

  double scale = !enabled  ? gmButton.scale
                 : clicked ? gmButton.active.scale
                 : hovered ? gmButton.focussed.scale
                           : gmButton.scale;

  gmColor bg = !enabled  ? gmButton.disabled.background
               : clicked ? gmButton.active.background
               : hovered ? gmButton.focussed.background
                         : gmButton.background;

  gmColor border = !enabled  ? gmButton.disabled.border
                   : clicked ? gmButton.active.border
                   : hovered ? gmButton.focussed.border
                             : gmButton.border;

  gmColor fg = !enabled  ? gmButton.disabled.text
               : clicked ? gmButton.active.text
               : hovered ? gmButton.focussed.text
                         : gmButton.text;

  // Scale expands symmetrically from the center
  double sw = width * scale;
  double sh = height * scale;

  // Border (square, centered)
  gm_draw_rectangle(x, y, sw + gmButton.border_thickness * 2,
                    sh + gmButton.border_thickness * 2, border);

  // Background (centered)
  gm_draw_rectangle(x, y, sw, sh, bg);

  // Text (already center-based)
  if (text) {
    gm_draw_text(x, y, text, gmButton.font, fontsize * scale, fg);
  }

  return enabled && hovered;
}
