#pragma once

#include "../position.h"
#include "light.h"

typedef struct {
  gmPos viewport;
  double near;
  double far;
  gm3Light light;
  gm3Pos camera_pos;
  double ambient;
} gm3Scene;

int gm3_scene_create(gm3Scene *s, double w, double h) {
  s->viewport.x = w;
  s->viewport.y = h;
  s->near = 0.1;
  s->far = 100;
  s->light.color = GM_WHITE;
  s->light.direction = gm3pos(0, -1, 1);
  s->light.intensity = 1;
  s->light.position = gm3pos(0, 1, 0);
  gm3_pos_reset(&s->camera_pos);
  s->ambient = 1;
  return 0;
}

gm3Scene gm3_scene() {
  gm3Scene s;
  gm3_scene_create(&s, 2, 2);
  return s;
}

int gm3_free_scene(gm3Scene *s) { return 0; }
