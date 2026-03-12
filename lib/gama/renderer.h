/**
 * @file renderer.h
 * @brief Core software rasterizer for Gama.
 *
 * This file implements immediate-mode software rendering primitives that
 * operate directly on a pixel buffer (`gmScreen`).
 */

#ifndef GAMA_RENDERER_H_INCLUDED
#define GAMA_RENDERER_H_INCLUDED

#include "color.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Structure representing a virtual screen pixel buffer.
 */
typedef struct {
  int width, height;
  uint8_t *data; // RGB data (3 bytes per pixel)
} gmScreen;

/**
 * @brief Clears the screen with a specific color.
 */
static inline void gmsc_clear(gmScreen *s, gmColor c) {
  if (!s || !s->data)
    return;
  if (c == GM_BLACK) {
    memset(s->data, 0, s->width * s->height * 3);
  } else if (c == GM_WHITE) {
    memset(s->data, 255, s->width * s->height * 3);
  } else {
    uint8_t r = gm_red(c), g = gm_green(c), b = gm_blue(c);
    size_t max = s->width * s->height * 3;
    for (int i = 0; i < max; i += 3) {
      s->data[i + 0] = r;
      s->data[i + 1] = g;
      s->data[i + 2] = b;
    }
  }
}

/**
 * @brief Sets a pixel color directly (no alpha blending, no bounds check).
 */
static inline void gmsc_set_pixel(gmScreen *s, int x, int y, gmColor c) {
  int idx = 3 * (y * s->width + x);
  s->data[idx + 0] = gm_red(c);
  s->data[idx + 1] = gm_green(c);
  s->data[idx + 2] = gm_blue(c);
}

/**
 * @brief Gets a pixel color.
 */
static inline gmColor gmsc_get_pixel(gmScreen *s, int x, int y) {
  int idx = 3 * (y * s->width + x);
  return gm_rgb(s->data[idx + 0], s->data[idx + 1], s->data[idx + 2]);
}

/**
 * @brief Draws a pixel with alpha blending (no bounds check).
 */
static inline void gmsc_draw_pixel(gmScreen *s, int x, int y, gmColor c) {
  uint8_t a = gm_alpha(c);
  if (a == 0)
    return;
  if (a == 255) {
    gmsc_set_pixel(s, x, y, c);
    return;
  }

  int idx = 3 * (y * s->width + x);
  float alpha = (float)a / 255.0f;
  float inv_alpha = 1.0f - alpha;

  s->data[idx + 0] =
      (uint8_t)(s->data[idx + 0] * inv_alpha + gm_red(c) * alpha);
  s->data[idx + 1] =
      (uint8_t)(s->data[idx + 1] * inv_alpha + gm_green(c) * alpha);
  s->data[idx + 2] =
      (uint8_t)(s->data[idx + 2] * inv_alpha + gm_blue(c) * alpha);
}

/**
 * @brief Draws a pixel with bounds checking and alpha blending.
 */
static inline void gmsc_put_pixel(gmScreen *s, int x, int y, gmColor c) {
  if (x >= 0 && x < s->width && y >= 0 && y < s->height) {
    gmsc_draw_pixel(s, x, y, c);
  }
}

// --- Primitives ---

/**
 * @brief Draws a filled rectangle.
 */
void gmsc_fill_rect(gmScreen *s, gmColor c, int x, int y, int w, int h) {
  if (!s || !s->data)
    return;

  int x1 = x - w / 2, y1 = y - h / 2;
  int x2 = x1 + w, y2 = y1 + h;

  // Clipping
  if (x1 < 0)
    x1 = 0;
  if (y1 < 0)
    y1 = 0;
  if (x2 > s->width)
    x2 = s->width;
  if (y2 > s->height)
    y2 = s->height;

  for (int cy = y1; cy < y2; cy++) {
    for (int cx = x1; cx < x2; cx++) {
      gmsc_draw_pixel(s, cx, cy, c);
    }
  }
}

/**
 * @brief Draws a line using Bresenham's algorithm.
 */
void gmsc_draw_line(gmScreen *s, gmColor c, int x0, int y0, int x1, int y1) {
  int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
  int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
  int err = dx + dy, e2;

  while (1) {
    gmsc_put_pixel(s, x0, y0, c);
    if (x0 == x1 && y0 == y1)
      break;
    e2 = 2 * err;
    if (e2 >= dy) {
      err += dy;
      x0 += sx;
    }
    if (e2 <= dx) {
      err += dx;
      y0 += sy;
    }
  }
}

/**
 * @brief Draws a filled circle using the midpoint circle algorithm.
 */
void gmsc_fill_circle(gmScreen *s, gmColor c, int cx, int cy, int r) {
  int x = r, y = 0;
  int err = 0;

  while (x >= y) {
    // Draw horizontal lines between octants to fill
    for (int i = cx - x; i <= cx + x; i++) {
      gmsc_put_pixel(s, i, cy + y, c);
      gmsc_put_pixel(s, i, cy - y, c);
    }
    for (int i = cx - y; i <= cx + y; i++) {
      gmsc_put_pixel(s, i, cy + x, c);
      gmsc_put_pixel(s, i, cy - x, c);
    }

    if (err <= 0) {
      y += 1;
      err += 2 * y + 1;
    }
    if (err > 0) {
      x -= 1;
      err -= 2 * x + 1;
    }
  }
}

/**
 * @brief Helper for triangle rasterization: fills a flat-bottom triangle.
 */
static void gmsc_fill_flat_bottom_triangle(gmScreen *s, gmColor c, int x0,
                                           int y0, int x1, int y1, int x2,
                                           int y2) {
  float invslope1 = (float)(x1 - x0) / (y1 - y0);
  float invslope2 = (float)(x2 - x0) / (y2 - y0);
  float curx1 = x0, curx2 = x0;
  for (int scanlineY = y0; scanlineY <= y1; scanlineY++) {
    for (int x = (int)curx1; x <= (int)curx2; x++)
      gmsc_put_pixel(s, x, scanlineY, c);
    curx1 += invslope1;
    curx2 += invslope2;
  }
}

/**
 * @brief Helper for triangle rasterization: fills a flat-top triangle.
 */
static void gmsc_fill_flat_top_triangle(gmScreen *s, gmColor c, int x0, int y0,
                                        int x1, int y1, int x2, int y2) {
  float invslope1 = (float)(x2 - x0) / (y2 - y0);
  float invslope2 = (float)(x2 - x1) / (y2 - y1);
  float curx1 = x2, curx2 = x2;
  for (int scanlineY = y2; scanlineY > y0; scanlineY--) {
    for (int x = (int)curx1; x <= (int)curx2; x++)
      gmsc_put_pixel(s, x, scanlineY, c);
    curx1 -= invslope1;
    curx2 -= invslope2;
  }
}

/**
 * @brief Draws a filled triangle.
 */
void gmsc_fill_triangle(gmScreen *s, gmColor c, int x0, int y0, int x1, int y1,
                        int x2, int y2) {
  // Sort vertices by Y
  if (y0 > y1) {
    int tx = x0;
    x0 = x1;
    x1 = tx;
    int ty = y0;
    y0 = y1;
    y1 = ty;
  }
  if (y1 > y2) {
    int tx = x1;
    x1 = x2;
    x2 = tx;
    int ty = y1;
    y1 = y2;
    y2 = ty;
  }
  if (y0 > y1) {
    int tx = x0;
    x0 = x1;
    x1 = tx;
    int ty = y0;
    y0 = y1;
    y1 = ty;
  }

  if (y1 == y2) {
    gmsc_fill_flat_bottom_triangle(s, c, x0, y0, x1, y1, x2, y2);
  } else if (y0 == y1) {
    gmsc_fill_flat_top_triangle(s, c, x0, y0, x1, y1, x2, y2);
  } else {
    int x3 = (int)(x0 + ((float)(y1 - y0) / (float)(y2 - y0)) * (x2 - x0));
    gmsc_fill_flat_bottom_triangle(s, c, x0, y0, x1, y1, x3, y1);
    gmsc_fill_flat_top_triangle(s, c, x1, y1, x3, y1, x2, y2);
  }
}

#endif // GAMA_RENDERER_H_INCLUDED
