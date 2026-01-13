/**
 * @file camera.h
 * @brief Defines the 3D camera structure for scene projection.
 */
#pragma once

/**
 * @brief Represents a camera in a 3D scene, used for projection.
 */
typedef struct {
  double focal; /**< The focal length of the camera, affecting perspective. */
  double near;  /**< The distance to the near clipping plane. Objects closer than this are clipped. */
  double far;   /**< The distance to the far clipping plane. Objects farther than this are clipped. */
} gm3Camera;

/**
 * @brief A default `gm3Camera` instance.
 *
 * Initializes a camera with a typical focal length and clipping planes.
 */
const gm3Camera gm3_default_camera = {
    .far = 100,
    .near = 0.01,
    .focal = 1.3,
};

/**
 * @brief Initializes a `gm3Camera` struct with default values.
 * @param c A pointer to the `gm3Camera` struct to initialize.
 * @return 0 on success, -1 if `c` is NULL.
 */
int gm3_camera_create(gm3Camera *c) {
  if (!c)
    return -1;
  *c = gm3_default_camera;
  return 0;
}
