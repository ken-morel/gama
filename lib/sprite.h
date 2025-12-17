#pragma once
#include "image.h"
#include <stdlib.h>
#ifndef GAMA_MAX_SPRITE_ANIM_LENGTH
#define GAMA_MAX_SPRITE_ANIM_LENGTH 10
#endif

typedef struct {
  double interval;
  int anim[GAMA_MAX_SPRITE_ANIM_LENGTH];
  size_t length;
} gmSpriteAnim;

gmSpriteAnim gm_sprite_anim_browse(int n_sprites, double interval) {
  gmSpriteAnim anim;
  anim.interval = interval;
  anim.length = n_sprites;
  for (int i = 0; i < n_sprites; i++)
    anim.anim[i] = i;

  return anim;
}
gmSpriteAnim gm_sprite_anim_create(double interval, const char *pattern) {
  gmSpriteAnim anim;
  int i;

  for (i = 0; pattern[i] != '\0'; i++) {
    anim.anim[i] = (int)pattern[i] - (int)'a';
  }
  anim.length = i;
  anim.interval = interval;
  return anim;
}

typedef struct {
  double _backlog_t;

  gmSpriteAnim animation;

  size_t n_frames;
  size_t animation_frame;
  size_t _frame;

  gmImage image;
} gmSprite;

gmSprite gm_sprite_create(gmImage img, int n_frames) {
  gmSprite s;
  s._backlog_t = 0;
  s.n_frames = n_frames;
  s.animation = gm_sprite_anim_browse(n_frames, 0.1);
  s.image = img;
  s.animation_frame = 0;
  s._frame = 0;
  return s;
}
void gm_sprite_update_dt(gmSprite *sprite, double dt) {
  sprite->_backlog_t += dt;
  while (sprite->_backlog_t >= sprite->animation.interval) {
    sprite->_backlog_t -= sprite->animation.interval;
    sprite->animation_frame++;
    sprite->animation_frame %= sprite->animation.length;
    sprite->_frame = sprite->animation.anim[sprite->animation_frame];
  }
}
static inline void gm_sprite_update(gmSprite *sprite) {
  gm_sprite_update_dt(sprite, gm_dt());
}
void gm_sprite_draw(gmSprite *sprite, double x, double y, double width,
                    double height) {
  size_t im_w = sprite->image.width / sprite->n_frames;
  gm_image_draw_part(sprite->image, im_w * sprite->_frame, 0, im_w,
                     sprite->image.height, x, y, width, height);
}
gmSprite gm_sprite_open(const char *path, int n_frames) {
  return gm_sprite_create(gm_image_open(path), n_frames);
}
