#pragma once
#include <image.h>
#include <stdint.h>

typedef struct {
  double interval;
  double last_time;

  uint8_t pattern[10];
  uint8_t frame_index;

  uint32_t width;
  uint32_t height;
  uint8_t n_sprites;

  gmImage image;
} gmSprite;
