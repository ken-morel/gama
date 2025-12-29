#pragma once

#include "../position.h"
#include "camera.h"
#include "light.h"

typedef struct {
  gmPos viewport;
  double near;
  double far;
  gm3Light light;
  gm3Camera camera;
} gm3Scene;

int gm3_scene_create(gm3Scene *s, double w, double h) {
  s->viewport.x = w;
  s->viewport.y = h;
  s->near = 0.1;
  s->far = 100;
  gm3_light_create(&s->light);
  gm3_camera_create(&s->camera);
  return 0;
}

gm3Scene gm3_scene() {
  gm3Scene s;
  gm3_scene_create(&s, 2, 2);
  return s;
}

int gm3_scene_free(gm3Scene *s) { return 0; }
