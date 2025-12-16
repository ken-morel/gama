#pragma once

#include "body_list.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h> // For malloc and free

/*
General notes on animation functions:
- value:  A pointer to the variable to be animated.
- target: The target value to animate towards.
- dt:     Delta time - the time elapsed since the last frame (in seconds).
- t:      The approximate time the animation should take (in seconds). It acts
as a time constant.
*/

/**
 * @brief Moves a value towards a target with spring-like motion (exponential
 * ease-out).
 * @param t The animation's approximate duration. A smaller 't' results in a
 * faster animation.
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

// Struct to hold the state of a single linear animation instance.
typedef struct {
  double *value_ptr;   // Pointer to the double being animated
  double start_value;  // The value when the animation started
  double target_value; // The target value of the animation
  double elapsed_time; // Time elapsed since the animation started
  double total_time;   // The total duration of the animation
} gmAnimLinearState;

/**
 * @brief Moves a value towards a target at a constant speed over a specified
 * duration.
 *
 * This function is stateful. It tracks all ongoing linear animations and will
 * correctly interpolate the value over the specified time 't'. If the target
 * value changes for a given pointer, the animation will restart.
 *
 * @param value A pointer to the double value to animate.
 * @param target The target value to animate towards.
 * @param dt Delta time since the last frame.
 * @param t The total time the animation should take in seconds.
 */
void gm_anim_linear(double *value, double target, const double dt,
                    const double t) {
  if (value == NULL || t <= 0)
    return;

  // List of active linear animations.
  static gmPtrList active_anims = NULL;

  gmAnimLinearState *state = NULL;
  int state_idx = -1;
  size_t list_len = gm_ptr_list_length(active_anims);

  // Find if an animation for this 'value' pointer already exists.
  for (size_t i = 0; i < list_len; i++) {
    gmAnimLinearState *s = (gmAnimLinearState *)active_anims[i];
    if (s->value_ptr == value) {
      state = s;
      state_idx = i;
      break;
    }
  }

  // If the target has changed, we need to restart the animation.
  if (state && state->target_value != target) {
    // Free the old state and remove it from the list.
    free(state);
    active_anims = gm_ptr_list_pop_at(active_anims, state_idx);
    state = NULL; // Force creation of a new state.
  }

  // If no state exists, create a new one.
  if (state == NULL) {
    // Don't start a new animation if it's already at the target.
    if (*value == target)
      return;

    state = (gmAnimLinearState *)malloc(sizeof(gmAnimLinearState));
    state->value_ptr = value;
    state->start_value = *value;
    state->target_value = target;
    state->elapsed_time = 0.0;
    state->total_time = t;
    active_anims = gm_ptr_list_push(active_anims, state);
  }

  // Update the animation state.
  state->elapsed_time += dt;

  // Check for completion.
  if (state->elapsed_time >= state->total_time) {
    *value = state->target_value;
    // Find the index again in case it was just added.
    state_idx = gm_ptr_list_find(active_anims, state);
    if (state_idx != -1) {
      // Remove the state from the list and free memory.
      active_anims = gm_ptr_list_pop_at(active_anims, state_idx);
    }
    free(state);
  } else {
    // Perform linear interpolation (lerp).
    double fraction = state->elapsed_time / state->total_time;
    *value = state->start_value +
             (state->target_value - state->start_value) * fraction;
  }
}

/**
 * @brief Starts fast and decelerates quadratically to the target. More
 * pronounced than spring.
 * @param t The animation's approximate duration.
 */
void gm_anim_ease_out_quad(double *value, const double target, const double dt,
                           double t) {
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
 * @brief Starts very fast and decelerates cubically to the target. More
 * pronounced than quad.
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

static inline double gm_anim_sin(double center, double radius, double speed,
                                 double offset) {
  return center + (radius * sin(speed * (gm_t() + offset) * M_PI * 2));
}

static inline double gm_anim_cos(double center, double radius, double speed,
                                 double offset) {
  return center + (radius * cos(speed * (gm_t() + offset) * M_PI * 2));
}
