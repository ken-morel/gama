/**
 * @file sprite.h
 * @brief Defines structures and functions for managing 2D sprites and animations.
 *
 * This file provides utilities for creating animated sprites from sprite sheets,
 * controlling their animation, and drawing them to the screen.
 */
#ifndef GM_SPRITE_H_INCLUDED
#define GM_SPRITE_H_INCLUDED
#include "image.h"
#include <stdint.h>
#include <stdlib.h>
/**
 * @def GAMA_MAX_SPRITE_ANIM_LENGTH
 * @brief Defines the maximum number of frames an animation sequence can have.
 */
#ifndef GAMA_MAX_SPRITE_ANIM_LENGTH
#define GAMA_MAX_SPRITE_ANIM_LENGTH 10
#endif

/**
 * @brief Structure representing a sprite animation sequence.
 */
typedef struct {
  double interval; /**< Time interval in seconds between animation frames. */
  int8_t anim[GAMA_MAX_SPRITE_ANIM_LENGTH]; /**< Array of frame indices in the
                                            animation sequence. These are indices
                                            into the sprite sheet's total frames. */
  size_t length; /**< Number of frames in the animation sequence. */
} gmSpriteAnim;

/**
 * @brief Creates a sprite animation that sequentially browses through all available frames.
 *
 * This function creates a simple animation where each frame of the sprite sheet
 * is played in sequential order from 0 to `n_sprites - 1`.
 *
 * @param n_sprites The total number of frames in the sprite sheet.
 * @param interval The time interval in seconds between each frame.
 * @return A new `gmSpriteAnim` instance.
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
 *
 * Each character in the `pattern` string corresponds to a frame index,
 * where 'a' maps to frame 0, 'b' to frame 1, and so on. This allows for
 * custom animation sequences.
 *
 * @param interval The time interval in seconds between frames.
 * @param pattern A string where each character represents a frame index (e.g., "abcba" for a looping animation).
 * @return A new `gmSpriteAnim` instance.
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
 *
 * A sprite consists of an image (sprite sheet) and defines how to play
 * an animation sequence from that sheet.
 */
typedef struct {
  double _backlog_t; /**< Internal time accumulator for animation timing. */

  gmSpriteAnim animation; /**< The animation sequence for the sprite. */

  size_t n_frames;        /**< Total number of frames in the sprite sheet. */
  size_t animation_frame; /**< Current frame index in the `animation.anim` sequence. */
  size_t _frame;          /**< Current actual frame index from the sprite sheet to display. */

  gmImage image; /**< The image containing the sprite sheet. */
} gmSprite;

/**
 * @brief Creates a new sprite from an image with a specified number of frames.
 *
 * This function initializes a sprite from a `gmImage` that is assumed to be
 * a sprite sheet. The `n_frames` parameter specifies how many equal-width
 * frames are contained horizontally within the `image`.
 *
 * @param img The `gmImage` containing the sprite sheet.
 * @param n_frames The number of animation frames horizontally arranged in the sprite sheet.
 * @return A new `gmSprite` instance.
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
 * @brief Draws the current frame of a sprite at the specified position and
 * size.
 *
 * This function automatically calculates the correct slice of the sprite sheet
 * to draw based on the current animation frame.
 *
 * @param sprite Pointer to the sprite to draw.
 * @param x The x-coordinate of the center of the sprite.
 * @param y The y-coordinate of the center of the sprite.
 * @param width The width to draw the sprite.
 * @param height The height to draw the sprite.
 */
void gm_sprite_draw(gmSprite *sprite, double x, double y, double width,
                    double height) {
  size_t im_w = sprite->image.width / sprite->n_frames;
  gm_image_draw_part(sprite->image, im_w * sprite->_frame, 0, im_w,
                     sprite->image.height, x, y, width, height);
}

#endif // GM_SPRITE_H_INCLUDED
