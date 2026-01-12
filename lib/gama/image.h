#pragma once
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "../stb/stb_image.h"
#include <stdint.h>
#include <string.h>
#include "color.h"

/**
 * @brief A container for raw, CPU-side image pixel data.
 *
 * This struct holds raw pixel data decoded from an image file. It is typically
 * used as an intermediate step before creating a `gmImage`, which manages the
 * texture on the GPU.
 */
typedef struct {
  int32_t width, height; /**< Dimensions of the image in pixels. */
  unsigned char *data;  /**< Pointer to the raw RGBA pixel data. */
} gmImageData;

/**
 * @brief Loads image file from disk into a gmImageData struct.
 * @param data A pointer to the gmImageData struct to be filled.
 * @param path The file path of the image to load.
 * @return 0 on success, -1 on failure.
 */
int32_t gm_image_data_load(gmImageData *data, const char *path) {
  memset(data, 0, sizeof(gmImageData));
  data->data = stbi_load(path, &data->width, &data->height, NULL, 4);
  return data->data ? 0 : -1;
}

/**
 * @brief Loads image data from an in-memory buffer.
 * @param data Pointer to the gmImageData structure to fill.
 * @param buffer Pointer to the buffer containing the raw image file data.
 * @param len The length of the buffer in bytes.
 * @return 0 on success, -1 on failure.
 */
int32_t gm_image_data_load_from_memory(gmImageData *data,
                                       const unsigned char *buffer, int len) {
  memset(data, 0, sizeof(gmImageData));
  data->data =
      stbi_load_from_memory(buffer, len, &data->width, &data->height, NULL, 4);
  return data->data ? 0 : -1;
}

/**
 * @brief Frees the pixel data buffer of a gmImageData struct.
 * @param d A pointer to the gmImageData struct to free.
 * @return 0 on success.
 */
int32_t gm_image_data_free(gmImageData *d) {
  free(d->data);
  d->width = 0;
  d->height = 0;
  return 0;
}

/**
 * @brief Calculates the average color of an image's raw pixel data.
 * @param data Pointer to the gmImageData structure.
 * @return The average color as a gmColor.
 */
gmColor gm_image_data_average_color(const gmImageData *data) {
    if (!data || !data->data || data->width <= 0 || data->height <= 0) {
        return 0; // Return black for invalid data
    }

    long long total_r = 0;
    long long total_g = 0;
    long long total_b = 0;
    size_t pixel_count = data->width * data->height;

    for (size_t i = 0; i < pixel_count; ++i) {
        total_r += data->data[i * 4 + 0];
        total_g += data->data[i * 4 + 1];
        total_b += data->data[i * 4 + 2];
    }

    unsigned char avg_r = (unsigned char)(total_r / pixel_count);
    unsigned char avg_g = (unsigned char)(total_g / pixel_count);
    unsigned char avg_b = (unsigned char)(total_b / pixel_count);

    return gm_rgb(avg_r, avg_g, avg_b);
}


/**
 * @brief A handle to a GPU-managed image or texture.
 *
 * This struct represents an image that has been uploaded to the graphics
 * hardware for efficient rendering.
 */
typedef struct {
  uint32_t handle; /**< Internal handle for the image */
  int width;       /**< Width of the image in pixels */
  int height;      /**< Height of the image in pixels */
} gmImage;

#ifndef GM_NO_GAPI
#include "gapi.h"

/**
 * @brief Creates a GPU-managed image from a file path.
 *
 * This function loads an image file from disk, uploads its data to the GPU,
 * and then discards the CPU-side copy.
 *
 * @param path The file path to the image.
 * @return A `gmImage` handle.
 */
gmImage gm_image_create(const char *path) {
  gmImage img;
  gmImageData data;
  gm_image_data_load(&data, path);
  img.width = data.width;
  img.height = data.height;
  img.handle = gapi_create_image(data.data, img.width, img.height);
  gm_image_data_free(&data);
  return img;
}

/**
 * @brief Creates a GPU-managed image from in-memory data.
 *
 * This function decodes an image from a memory buffer, uploads its data to the
 * GPU, and then discards the CPU-side copy.
 *
 * @param buffer Pointer to the buffer containing the raw image file data.
 * @param len The length of the buffer in bytes.
 * @return A `gmImage` handle.
 */
gmImage gm_image_create_from_memory(const unsigned char *buffer, int len) {
  gmImage img;
  gmImageData data;
  gm_image_data_load_from_memory(&data, buffer, len);
  img.width = data.width;
  img.height = data.height;
  img.handle = gapi_create_image(data.data, img.width, img.height);
  gm_image_data_free(&data);
  return img;
}

/**
 * @brief Draws an entire image, centered at the specified position.
 * @param i The image to draw.
 * @param x The x-coordinate of the center of the image.
 * @param y The y-coordinate of the center of the image.
 * @param w The width to draw the image.
 * @param h The height to draw the image.
 */
void gm_image_draw(gmImage i, double x, double y, double w, double h) {
  gapi_draw_image(i.handle, x, y, w, h);
}

/**
 * @brief Draws a rectangular sub-region of an image.
 * @param i The source image to draw from.
 * @param slice_x The x-coordinate of the top-left corner of the sub-region.
 * @param slice_y The y-coordinate of the top-left corner of the sub-region.
 * @param slice_width The width of the sub-region.
 * @param slice_height The height of the sub-region.
 * @param x The x-coordinate of the center of the destination rectangle.
 * @param y The y-coordinate of the center of the destination rectangle.
 * @param w The width to draw the sub-region.
 * @param h The height to draw the sub-region.
 */
void gm_image_draw_part(gmImage i, int slice_x, int slice_y, int slice_width,
                        int slice_height, double x, double y, double w,
                        double h) {
  gapi_draw_image_part(i.handle, slice_x, slice_y, slice_width, slice_height, x,
                       y, w, h);
}
#endif

