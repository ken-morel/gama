#pragma once

#include "gapi.h"
#include <math.h>
#include <stdio.h>

/**
 * @file animate.h
 * @brief Functions for animating values with various easing functions.
 *
 * General notes on animation functions:
 * - value:  A pointer to the variable to be animated.
 * - target: The target value to animate towards.
 * - t:      The approximate time the animation should take (in seconds). It
 * acts as a time constant.
 */

/**
 * @brief Moves a value towards a target with spring-like motion (exponential
 * ease-out).
 * @param value A pointer to the double value to animate.
 * @param target The target value to animate towards.
 * @param t The animation's approximate duration. A smaller 't' results in a
 * faster animation.
 */
void gm_anim_spring(double *value, double target, double t) {
  if (value == NULL || t <= 0)
    return;
  double difference = target - *value;
  double move = (gm_dt() * difference) / t;
  if (fabs(move) >= fabs(difference)) {
    *value = target;
  } else {
    *value += move;
  }
}

/**
 * @brief Starts fast and decelerates quadratically to the target. More
 * pronounced than spring.
 * @param value A pointer to the double value to animate.
 * @param target The target value to animate towards.
 * @param t The animation's approximate duration.
 */
void gm_anim_ease_out_quad(double *value, const double target, double t) {
  if (value == NULL || t <= 0)
    return;
  double difference = target - *value;
  double speed_factor = 1.0 + sqrt(fabs(difference));
  double move = (gm_dt() * difference * speed_factor) / t;

  if (fabs(move) >= fabs(difference)) {
    *value = target;
  } else {
    *value += move;
  }
}

/**
 * @brief Starts very fast and decelerates cubically to the target. More
 * pronounced than quad.
 * @param value A pointer to the double value to animate.
 * @param target The target value to animate towards.
 * @param t The animation's approximate duration.
 */
void gm_anim_ease_out_cubic(double *value, double target, double t) {
  if (value == NULL || t <= 0)
    return;
  double difference = target - *value;
  double speed_factor = 1.0 + fabs(difference);
  double move = (gm_dt() * difference * speed_factor) / t;

  if (fabs(move) >= fabs(difference)) {
    *value = target;
  } else {
    *value += move;
  }
}

/**
 * @brief Starts slow and accelerates quadratically towards the target.
 * @param value A pointer to the double value to animate.
 * @param target The target value to animate towards.
 * @param t The animation's approximate duration.
 */
void gm_anim_ease_in_quad(double *value, double target, double t) {
  if (value == NULL || t <= 0)
    return;
  double difference = target - *value;
  if (fabs(difference) < 0.0001) {
    *value = target;
    return;
  }
  double speed_factor = 1.0 / (1.0 + sqrt(fabs(difference)));
  double move = (gm_dt() * difference * speed_factor) / t;

  // For ease-in, it's possible for the calculated move to be tiny, so we ensure
  // minimum progress.
  if (fabs(move) < 0.0001) {
    move = 0.0001 * (difference > 0 ? 1 : -1);
  }

  if (fabs(move) >= fabs(difference)) {
    *value = target;
  } else {
    *value += move;
  }
}

/**
 * @brief Returns a sinusoidal animation value based on time.
 * @param center The center value around which the animation oscillates.
 * @param radius The amplitude of the oscillation.
 * @param speed The speed of the oscillation.
 * @param offset The phase offset for the oscillation.
 * @return The current animated value based on a sine wave.
 */
static inline double gm_anim_sin(double center, double radius, double speed,
                                 double offset) {
  return center + (radius * sin(speed * (gm_t() + offset) * M_PI * 2));
}

/**
 * @brief Returns a cosine animation value based on time.
 * @param center The center value around which the animation oscillates.
 * @param radius The amplitude of the oscillation.
 * @param speed The speed of the oscillation.
 * @param offset The phase offset for the oscillation.
 * @return The current animated value based on a cosine wave.
 */
static inline double gm_anim_cos(double center, double radius, double speed,
                                 double offset) {
  return center + (radius * cos(speed * (gm_t() + offset) * M_PI * 2));
}
