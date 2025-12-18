#pragma once

#include "gapi.h"
#include <stdint.h>

/**
 * @brief Structure representing an image with handle and dimensions.
 */
typedef struct {
  uint32_t handle;  /**< Internal handle for the image */
  uint32_t width;   /**< Width of the image in pixels */
  uint32_t height;  /**< Height of the image in pixels */
} gmImage;

/**
 * @brief Loads an image from a file path.
 * @param path The file path to the image.
 * @return A gmImage structure containing the loaded image and its properties.
 */
gmImage gm_image_open(const char *path) {
  gmImage img;
  img.handle = gapi_create_image(path, &img.width, &img.height);
  return img;
}

/**
 * @brief Draws an entire image at the specified position and size.
 * @param i The image to draw.
 * @param x The x-coordinate to draw at.
 * @param y The y-coordinate to draw at.
 * @param w The width to draw the image.
 * @param h The height to draw the image.
 */
void gm_image_draw(gmImage i, double x, double y, double w, double h) {
  gapi_draw_image(i.handle, x, y, w, h);
}

/**
 * @brief Draws a part of an image at the specified position and size.
 * @param i The image to draw from.
 * @param slice_x The x-coordinate of the slice to draw from the image.
 * @param slice_y The y-coordinate of the slice to draw from the image.
 * @param slice_width The width of the slice to draw from the image.
 * @param slice_height The height of the slice to draw from the image.
 * @param x The x-coordinate to draw at.
 * @param y The y-coordinate to draw at.
 * @param w The width to draw the slice.
 * @param h The height to draw the slice.
 */
void gm_image_draw_part(gmImage i, int slice_x, int slice_y, int slice_width,
                        int slice_height, double x, double y, double w,
                        double h) {
  gapi_draw_image_part(i.handle, slice_x, slice_y, slice_width, slice_height, x,
                       y, w, h);
}
