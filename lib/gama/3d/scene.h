/**
 * @file scene.h
 * @brief Defines the 3D scene structure, encompassing lights, cameras, and viewport settings.
 *
 * This file provides the `gm3Scene` structure to manage global properties
 * of a 3D rendering environment, making it easier to pass scene context
 * to projection and rendering functions.
 */
#pragma once

#include "../position.h"
#include "camera.h"
#include "light.h"

/**
 * @brief Represents a complete 3D scene, including its camera, lights, and viewport.
 */
typedef struct {
  gmPos viewport;   /**< The viewport dimensions (width, height) for rendering. */

  gm3Light light;   /**< The primary light source in the scene. */
  gm3Camera camera; /**< The camera used for viewing the scene. */
} gm3Scene;

/**
 * @brief A default `gm3Scene` instance.
 *
 * Initializes a scene with a default viewport (2,2), a default light, and a default camera.
 */
const gm3Scene gm3_default_scene = {
    .viewport = {2, 2},
    .light = gm3_default_light,
    .camera = gm3_default_camera,
};

/**
 * @brief Initializes a `gm3Scene` struct with default values and a specified viewport.
 * @param s A pointer to the `gm3Scene` struct to initialize.
 * @param w The width of the viewport.
 * @param h The height of the viewport.
 * @return 0 on success, -1 if `s` is NULL.
 */
int gm3_scene_create(gm3Scene *s, double w, double h) {
  if (s == NULL)
    return -1;
  *s = gm3_default_scene;
  s->viewport.x = w;
  s->viewport.y = h;

  gm3_light_create(&s->light);
  gm3_camera_create(&s->camera);
  return 0;
}

/**
 * @brief Creates and returns a new `gm3Scene` struct initialized with default values.
 * @return A new `gm3Scene` instance.
 */
gm3Scene gm3_scene() { return gm3_default_scene; }

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
int gm3_scene_free(gm3Scene *s) { (void)s; return 0; }
