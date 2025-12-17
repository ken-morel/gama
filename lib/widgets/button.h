#include "../collision.h"
#include "../draw.h"

typedef struct {
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
} gmwButtonTheme;
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
                            .font = "default-ui"};

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
    gm_draw_text(x, y, text, gmwButton.font, fontsize * scale, fg);
  }

  return hovered;
}
