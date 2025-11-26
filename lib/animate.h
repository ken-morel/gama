#pragma once

#include <math.h>
#include <stdio.h>

/*
General notes on animation functions:
- value:  A pointer to the variable to be animated.
- target: The target value to animate towards.
- dt:     Delta time - the time elapsed since the last frame (in seconds).
- t:      The approximate time the animation should take (in seconds). It acts as a time constant.
*/

/**
 * @brief Moves a value towards a target with spring-like motion (exponential ease-out).
 * @param t The animation's approximate duration. A smaller 't' results in a faster animation.
 */
void gm_anim_spring(double *value, double target, double dt, double t) {
  if (value == NULL || t <= 0)
    return;
  double difference = target - *value;
  double move = (dt * difference) / t;
  if (fabs(move) >= fabs(difference)) {
    *value = target;
  } else {
    *value += move;
  }
}

/**
 * @brief Moves a value towards a target at a decreasing speed.
 * @note This is not a true linear animation (constant speed) due to the stateless nature
 * of the function. It behaves as an exponential ease-out, similar to gm_anim_spring.
 * @param t The animation's approximate duration.
 */
void gm_anim_linear(double *value, double target, double dt, double t) {
  gm_anim_spring(value, target, dt, t);
}

/**
 * @brief Starts fast and decelerates quadratically to the target. More pronounced than spring.
 * @param t The animation's approximate duration.
 */
void gm_anim_ease_out_quad(double *value, double target, double dt, double t) {
  if (value == NULL || t <= 0)
    return;
  double difference = target - *value;
  double speed_factor = 1.0 + sqrt(fabs(difference));
  double move = (dt * difference * speed_factor) / t;

  if (fabs(move) >= fabs(difference)) {
    *value = target;
  } else {
    *value += move;
  }
}

/**
 * @brief Starts very fast and decelerates cubically to the target. More pronounced than quad.
 * @param t The animation's approximate duration.
 */
void gm_anim_ease_out_cubic(double *value, double target, double dt, double t) {
  if (value == NULL || t <= 0)
    return;
  double difference = target - *value;
  double speed_factor = 1.0 + fabs(difference);
  double move = (dt * difference * speed_factor) / t;

  if (fabs(move) >= fabs(difference)) {
    *value = target;
  } else {
    *value += move;
  }
}

/**
 * @brief Starts slow and accelerates quadratically towards the target.
 * @param t The animation's approximate duration.
 */
void gm_anim_ease_in_quad(double *value, double target, double dt, double t) {
  if (value == NULL || t <= 0)
    return;
  double difference = target - *value;
  if (fabs(difference) < 0.0001) {
    *value = target;
    return;
  }
  double speed_factor = 1.0 / (1.0 + sqrt(fabs(difference)));
  double move = (dt * difference * speed_factor) / t;

  // For ease-in, it's possible for the calculated move to be tiny, so we ensure minimum progress.
  if (fabs(move) < 0.0001) {
      move = 0.0001 * (difference > 0 ? 1 : -1);
  }

  if (fabs(move) >= fabs(difference)) {
    *value = target;
  } else {
    *value += move;
  }
}
