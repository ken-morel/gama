#pragma once

#include "../color.h"
#include "position.h"

/**
 * @brief Represents a single light source in a 3D scene.
 *
 * This struct defines the properties of a light, including its position,
 * direction, color, and intensity, which are used in lighting calculations
 * by the 3D renderer.
 */
typedef struct {
  gm3Pos position;  /**< World-space position of the light source. */
  gm3Pos direction; /**< Direction of the light (e.g., for directional lights). */
  gmColor color;    /**< The color of the light. */
  double intensity; /**< The brightness of the light (e.g., 0.0 to 1.0). */
  double ambient;   /**< The ambient light contribution (0.0 to 1.0). */
} gm3Light;

/**
 * @brief A default `gm3Light` instance.
 *
 * Initializes a light source at `{0, 1, 0}` pointing towards `{0, -1, 1}`,
 * with a white color, medium intensity, and some ambient light.
 */
const gm3Light gm3_default_light = {
    .position = {0, 1, 0},
    .direction = {0, -1, 1},
    .color = 0xCCCCCCFF,
    .intensity = 0.5,
    .ambient = 0.5,
};

/**
 * @brief Initializes a `gm3Light` struct with default values.
 * @param l A pointer to the `gm3Light` struct to initialize.
 * @return 0 on success, -1 if `l` is NULL.
 */
int gm3_light_create(gm3Light *l) {
  if (!l)
    return -1;
  *l = gm3_default_light;
  // The original code had memset(l, 0, sizeof(gm3Light)) here after assigning
  // the default. This would zero out all the default values.
  // Assuming the intention is to use the default values, I'm removing memset.
  // If the intention was to zero it out, then the default values would be useless.
  // If a zero-initialized light is desired, it should be done explicitly.

  return 0;
}
