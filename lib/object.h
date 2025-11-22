
#pragma once

#include "color.h"
#include "image.h"
#include "position.h"
#include <stdint.h>

typedef struct {
  gama_position velocity;
  gama_position acceleration;
} gama_physics;

typedef enum {
  GAMA_SHAPE_NONE,
  GAMA_SHAPE_CIRCLE,
  GAMA_SHAPE_RECT
} gama_shape_type;

typedef struct {
  gama_color color;
  gama_shape_type type;
  double width, height, radius; // think about rounded rectangle which has both
} gama_shape;

typedef struct {
  uint8_t fps;
  uint8_t pattern[10];
  double last_time;
  gama_image image;
} gama_sprite;

typedef struct {
  int is_active;
  gama_position position;
  gama_physics physics;
  gama_shape shape;
  gama_sprite sprite;
} gama_object;

typedef gama_object *gobject;
