#ifndef GM3_LIGHT_H_INCLUDED
#define GM3_LIGHT_H_INCLUDED
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
  gm3Pos position; /**< World-space position of the light source. */
  gm3Pos
      direction; /**< Direction of the light (e.g., for directional lights). */
  gmColor color; /**< The color of the light. */
  double intensity; /**< The brightness of the light (e.g., 0.0 to 1.0). */
  double ambient;   /**< The ambient light contribution (0.0 to 1.0). */
} gm3Light;

#define gm3_default_light                                                      \
  (gm3Light) {                                                                 \
    .position = {0, 1, 0}, .direction = {0, -1, 1}, .color = 0xCCCCCCFF,       \
    .intensity = 0.5, .ambient = 0.5,                                          \
  }

/**
 * @brief A default `gm3Light` instance.
 *
 * Initializes a light source at `{0, 1, 0}` pointing towards `{0, -1, 1}`,
 * with a white color, medium intensity, and some ambient light.
 */
#define gm3_light() gm3_default_light
#endif // GM3_LIGHT_H_INCLUDED
