#pragma once
#include "image.h"
#include <stdlib.h>
#ifndef GAMA_MAX_SPRITE_ANIM_LENGTH
#define GAMA_MAX_SPRITE_ANIM_LENGTH 10
#endif

/**
 * @brief Structure representing a sprite animation sequence.
 */
typedef struct {
  double interval;  /**< Time interval between animation frames */
  int anim[GAMA_MAX_SPRITE_ANIM_LENGTH];  /**< Array of frame indices in the animation sequence */
  size_t length;    /**< Number of frames in the animation */
} gmSpriteAnim;

/**
 * @brief Creates a sprite animation that sequentially browses through frames.
 * @param n_sprites The number of sprites/frames in the animation.
 * @param interval The time interval between frames.
 * @return A new gmSpriteAnim instance.
 */
gmSpriteAnim gm_sprite_anim_browse(int n_sprites, double interval) {
  gmSpriteAnim anim;
  anim.interval = interval;
  anim.length = n_sprites;
  for (int i = 0; i < n_sprites; i++)
    anim.anim[i] = i;

  return anim;
}

/**
 * @brief Creates a sprite animation based on a pattern string.
 * @param interval The time interval between frames.
 * @param pattern A string where each character represents a frame index (a=0, b=1, etc.).
 * @return A new gmSpriteAnim instance.
 */
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

/**
 * @brief Structure representing a sprite with animation capabilities.
 */
typedef struct {
  double _backlog_t;       /**< Internal time accumulator for animation timing */

  gmSpriteAnim animation;  /**< Animation sequence for the sprite */

  size_t n_frames;         /**< Total number of frames in the sprite sheet */
  size_t animation_frame;  /**< Current frame in the animation sequence */
  size_t _frame;           /**< Current actual frame to display */

  gmImage image;           /**< The image containing the sprite sheet */
} gmSprite;

/**
 * @brief Creates a new sprite from an image with a specified number of frames.
 * @param img The image containing the sprite sheet.
 * @param n_frames The number of frames in the sprite sheet.
 * @return A new gmSprite instance.
 */
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

/**
 * @brief Updates the sprite's animation state based on elapsed time.
 * @param sprite Pointer to the sprite to update.
 * @param dt Delta time since the last update.
 */
void gm_sprite_update_dt(gmSprite *sprite, double dt) {
  sprite->_backlog_t += dt;
  while (sprite->_backlog_t >= sprite->animation.interval) {
    sprite->_backlog_t -= sprite->animation.interval;
    sprite->animation_frame++;
    sprite->animation_frame %= sprite->animation.length;
    sprite->_frame = sprite->animation.anim[sprite->animation_frame];
  }
}

/**
 * @brief Updates the sprite's animation state using the global delta time.
 * @param sprite Pointer to the sprite to update.
 */
static inline void gm_sprite_update(gmSprite *sprite) {
  gm_sprite_update_dt(sprite, gm_dt());
}

/**
 * @brief Draws the current frame of a sprite at the specified position and size.
 * @param sprite Pointer to the sprite to draw.
 * @param x The x-coordinate to draw at.
 * @param y The y-coordinate to draw at.
 * @param width The width to draw the sprite.
 * @param height The height to draw the sprite.
 */
void gm_sprite_draw(gmSprite *sprite, double x, double y, double width,
                    double height) {
  size_t im_w = sprite->image.width / sprite->n_frames;
  gm_image_draw_part(sprite->image, im_w * sprite->_frame, 0, im_w,
                     sprite->image.height, x, y, width, height);
}

/**
 * @brief Creates a new sprite by loading an image from a file.
 * @param path The file path to the sprite sheet image.
 * @param n_frames The number of frames in the sprite sheet.
 * @return A new gmSprite instance.
 */
gmSprite gm_sprite_open(const char *path, int n_frames) {
  return gm_sprite_create(gm_image_open(path), n_frames);
}
