#include "gapi.h"
#include "position.h"
#include <stdint.h>

struct gama_image {
  uint32_t handle;
  uint32_t image_width;
  uint32_t image_height;

  double width;
  double height;

  struct gama_position position;
  struct gama_position velocity;
  struct gama_position acceleration;
};

struct gama_image gama_image_create(const char *path) {
  struct gama_image img;
  img.handle = gapi_create_image(path, &img.image_width, &img.image_height);
  img.width = 1;
  img.height = 1;
  gama_position_set(&img.position, 0, 0);
  gama_position_set(&img.velocity, 0, 0);
  gama_position_set(&img.acceleration, 0, 0);
  return img;
}

void gama_image_update(struct gama_image *i, double dt) {
  gama_position_update(&i->position, &i->velocity, &i->acceleration, dt);
}

void gama_image_draw(struct gama_image *i) {
  gapi_draw_image(i->handle, i->position.x, i->position.y, i->width, i->height);
}

void gama_image_render(struct gama_image *i, double dt) {
  gama_image_update(i, dt);
  gama_image_draw(i);
}
