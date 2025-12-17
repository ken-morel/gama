#include "../animate.h"
#include "../collision.h"
#include "../draw.h"

struct {
  int enabled;

  double scale; // overall widget scale

  gmColor background; // track background
  gmColor border;     // track border

  struct {
    double scale; // hover scale
    gmColor border;
  } focussed;

  struct {
    double scale; // active scale (mouse down)
    gmColor border;
  } active;

  gmColor knob; // the slider knob
  gmColor knob_border;

  double border_width; // track border thickness
  double step;         // 0 = free, >0 = stepped increments
} gmScale = {.enabled = 1,
             .scale = 1.0,

             .background = 0x3A2A3AE0,
             .border = 0x7F4F7FFF,

             .focussed = {.scale = 1.03, .border = 0xAA77AAFF},
             .active = {.scale = 0.97, .border = 0x7F4F7FFF},

             .knob = 0xAA77AAFF,
             .knob_border = 0x6F3F6FFF,

             .border_width = 0.01,
             .step = 0.0};

int gm_scale_anim(double x, double y, double width, double height,
                  double *value, double *anim) {

  int enabled = gmScale.enabled;

  // Hover test uses full widget size (logical width/height)
  int hovered = enabled && gm_mouse_in_rect(x, y, width, height);
  int active = enabled && gm_mouse.down && hovered;

  if (anim == NULL)
    anim = value;
  else
    gm_anim_ease_out_quad(anim, (double)(*value), gm_dt(), 0.05);

  // Visual scale / border
  double scale = 1.0;
  gmColor border = gmScale.border;
  if (active) {
    scale = gmScale.active.scale;
    border = gmScale.active.border;
  } else if (hovered) {
    scale = gmScale.focussed.scale;
    border = gmScale.focussed.border;
  } else {
    scale = gmScale.scale;
  }

  // Draw sizes (scaled visually)
  double sw = width * scale;
  double sh = height * scale;

  // Draw track border
  gm_draw_rectangle(x, y, sw, sh, border);

  // Track inner size (for knob movement)
  double track_w = sw - gmScale.border_width * 2;
  double track_h = sh - gmScale.border_width * 2;

  // Draw track
  gm_draw_rectangle(x, y, track_w, track_h, gmScale.background);

  // Orientation
  int horizontal = (width >= height);

  // Handle dragging along logical track (not scaled)
  if (active) {
    if (horizontal) {
      double left = x - width * 0.5 + gmScale.border_width;
      double rel =
          (gm_mouse.position.x - left) / (width - gmScale.border_width * 2);
      if (rel < 0.0)
        rel = 0.0;
      if (rel > 1.0)
        rel = 1.0;
      if (gmScale.step > 0.0)
        rel = ((int)(rel / gmScale.step + 0.5)) * gmScale.step;
      *value = rel;
    } else {
      double bottom = y - height * 0.5 + gmScale.border_width;
      double rel =
          (gm_mouse.position.y - bottom) / (height - gmScale.border_width * 2);
      if (rel < 0.0)
        rel = 0.0;
      if (rel > 1.0)
        rel = 1.0;
      if (gmScale.step > 0.0)
        rel = ((int)(rel / gmScale.step + 0.5)) * gmScale.step;
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
  gm_draw_rectangle(knob_x, knob_y, knob_size + gmScale.border_width * 2,
                    knob_size + gmScale.border_width * 2, gmScale.knob_border);

  // Draw knob
  gm_draw_rectangle(knob_x, knob_y, knob_size, knob_size, gmScale.knob);

  return hovered;
}

static inline int gm_scale(double x, double y, double width, double height,
                           double *value) {
  return gm_scale_anim(x, y, width, height, value, NULL);
}
