#pragma once
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "../stb/stb_image.h"
#include <stdint.h>
#include <string.h>

typedef struct {
  int width, height;
  unsigned char *data;
} gmImageData;

int gm_image_data_load(gmImageData *data, const char *path) {
  memset(data, 0, sizeof(gmImageData));
  data->data = stbi_load(path, &data->width, &data->height, NULL, 4);
  return data->data ? 0 : -1;
}
int gm_image_data_free(gmImageData *d) {
  free(d->data);
  d->width = 0;
  d->height = 0;
  return 0;
}

/**
 * @brief Structure representing an image with handle and dimensions.
 */
typedef struct {
  uint32_t handle; /**< Internal handle for the image */
  int width;       /**< Width of the image in pixels */
  int height;      /**< Height of the image in pixels */
} gmImage;

#ifndef GM_NO_GAPI
#include "gapi.h"

/**
 * @brief Loads an image from a file path.
 * @param path The file path to the image.
 * @return A gmImage structure containing the loaded image and its properties.
 */
gmImage gm_image_create(const char *path) {
  gmImage img;
  gmImageData data;
  gm_image_data_load(&data, path);
  img.width = data.width;
  img.height = data.width;
  img.handle = gapi_create_image(data.data, img.width, img.height);
  gm_image_data_free(&data);
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
#endif
