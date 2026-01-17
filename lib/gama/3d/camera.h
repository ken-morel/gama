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
  double near_plane; /**< The distance to the near clipping plane. Objects
                  closer than this are clipped. */
  double far_plane; /**< The distance to the far clipping plane. Objects farther
                 than this are clipped. */
} gm3Camera;

/**
 * @brief A default `gm3Camera` instance.
 *
 * Initializes a camera with a typical focal length and clipping planes.
 */
static inline gm3Camera gm3_camera() {

  return (gm3Camera){.far_plane = 100, .near_plane = 0.01, .focal = 1.3};
}
