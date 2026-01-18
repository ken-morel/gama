/**
 * @file scene.h
 * @brief Defines the 3D scene structure, encompassing lights, cameras, and
 * viewport settings.
 *
 * This file provides the `gm3Scene` structure to manage global properties
 * of a 3D rendering environment, making it easier to pass scene context
 * to projection and rendering functions.
 */

 #ifndef GM3_SCENE_H_INCLUDED
 #define GM3_SCENE_H_INCLUDED
#include "../position.h"
#include "camera.h"
#include "light.h"

/**
 * @brief Represents a complete 3D scene, including its camera, lights, and
 * viewport.
 */
typedef struct {
  gmPos viewport; /**< The viewport dimensions (width, height) for rendering. */

  gm3Light light;   /**< The primary light source in the scene. */
  gm3Camera camera; /**< The camera used for viewing the scene. */
} gm3Scene;

/**
 * @brief Creates and returns a new `gm3Scene` struct initialized with default
 * values.
 * @return A new `gm3Scene` instance.
 */
static inline gm3Scene gm3_scene() {
  return (gm3Scene){
      .viewport = {2, 2},
      .light = gm3_light(),
      .camera = gm3_camera(),
  };
}

/**
 * @brief Frees any dynamically allocated memory associated with a `gm3Scene`.
 *
 * Currently, the `gm3Scene` struct does not directly manage any dynamically
 * allocated pointers that require explicit freeing, so this function is a
 * placeholder.
 *
 * @param s A pointer to the `gm3Scene` to free.
 * @return 0 on success.
 */
int gm3_scene_free(gm3Scene *s) {
  (void)s;
  return 0;
}
#endif
