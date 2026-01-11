#pragma once

#include "../position.h"
#include "camera.h"
#include "light.h"

typedef struct {
  gmPos viewport;

  gm3Light light;
  gm3Camera camera;
} gm3Scene;

const gm3Scene gm3_default_scene = {
    .viewport = {2, 2},
    .light = gm3_default_light,
    .camera = gm3_default_camera,
};

int gm3_scene_create(gm3Scene *s, double w, double h) {
  if (s == NULL)
    return -1;
  *s = gm3_default_scene;
  s->viewport.x = w;
  s->viewport.y = h;

  gm3_light_create(&s->light);
  gm3_camera_create(&s->camera);
  return 0;
}

gm3Scene gm3_scene() { return gm3_default_scene; }

int gm3_scene_free(gm3Scene *s) { return 0; }
