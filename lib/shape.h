#pragma once

#include "color.h"
#include "position.h"
#include "draw.h"

// ---------------------------------------------------------------------------
// --------------------------------- rectangle -------------------------------
// ---------------------------------------------------------------------------
struct gama_rectangle {

  double width;
  double height;
  Color color;
  struct gama_position position;
  struct gama_position velocity;
  struct gama_position acceleration;
};

void gama_rectangle_init(struct gama_rectangle *r, double x, double y, double w,
                         double h, Color c) {
  r->width = w;
  r->height = h;
  r->color = c;
  gama_position_set(&r->position, x, y);
  gama_position_set(&r->velocity, x, y);
  gama_position_set(&r->acceleration, x, y);
}
struct gama_rectangle gama_rectangle_create(double x, double y, double w,
                                            double h, Color c) {
  struct gama_rectangle r;
  gama_rectangle_init(&r, x, y, w, h, c);
  return r;
}

struct gama_rectangle gama_rectangle() {
  struct gama_rectangle r;
  gama_rectangle_init(&r, 0, 0, 1, 1, BLUE);
  return r;
}

void gama_rectangle_update(struct gama_rectangle *r, double dt) {
  gama_position_update(&r->position, &r->velocity, &r->acceleration, dt);

}

void gama_rectangle_render(struct gama_rectangle *r, double dt) {
  gama_rectangle_update(r,  dt);
}

void gama_rectangle_draw(struct gama_rectangle *r) {
  draw_rectantle(r->position.x, r->position.y, r->width, r->height,  r->color);
}
