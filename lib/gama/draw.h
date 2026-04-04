/**
 * @file draw.h
 * @brief Functions for drawing shapes, text, and images.
 *
 * This file provides a set of functions for immediate-mode rendering of
 * various primitives, as well as helper functions to draw physics bodies
 * (`gmBody`). All coordinates are in world space.
 */

#ifndef GM_DRAW_H_INCLUDED
#define GM_DRAW_H_INCLUDED
#include "body.h"
#include "color.h"
#include "gapi.h"
#include "image.h" // For gmImage
#include "renderer.h"
#include "shape.h"

#include <stdint.h>

#ifdef GM_BUILTIN_RENDERER
extern gmScreen _gm_screen;
#endif

static inline int32_t gm_clear() {
#ifdef GM_BUILTIN_RENDERER
  gmsc_clear(&_gm_screen, GM_BLACK); // Default clear to black in software mode
  return 0;
#elif !defined(GM_NO_GAPI_DRAW)
  return gapi_clear();
#else
  return -1;
#endif
}

// ---------------------------------------------------------------------------
// ------------------------- Immediate-Mode Primitives -----------------------
// ---------------------------------------------------------------------------

/**
 * @brief Draws a line segment.
 */
static inline int32_t gm_draw_line(double x1, double y1, double x2, double y2,
                                   double thickness, gmColor c) {
#ifdef GM_BUILTIN_RENDERER
  gmsc_draw_line(&_gm_screen, c, (int)x1, (int)y1, (int)x2, (int)y2);
  return 0;
#elif !defined(GM_NO_GAPI_DRAW)
  return gapi_draw_line(x1, y1, x2, y2, thickness, c);
#else
  return -1;
#endif
}

static inline int32_t gm_line(gmPos start, gmPos stop, double thickness,
                              gmColor color) {
  return gm_draw_line(start.x, start.y, stop.x, stop.y, thickness, color);
}

/**
 * @brief Draws a rectangle centered at a point.
 */
static inline int32_t gm_draw_rect(double x, double y, double w, double h,
                                   gmColor c) {
#ifdef GM_BUILTIN_RENDERER
  gmsc_fill_rect(&_gm_screen, c, (int)x, (int)y, (int)w, (int)h);
  return 0;
#elif !defined(GM_NO_GAPI_DRAW)
  return gapi_draw_rect(x, y, w, h, c);
#else
  return -1;
#endif
}
#define gm_draw_rectangle(x, y, w, h, c) gm_draw_rect(x, y, w, h, c)

static inline int32_t gm_rect(gmRect r, gmColor c) {
  return gm_draw_rect(r.pos.x, r.pos.y, r.size.x, r.size.y, c);
}

/**
 * @brief Draws a rectangle with rounded corners centered at a point.
 */
static inline int32_t gm_draw_roundrect(double x, double y, double w, double h,
                                        double r, gmColor c) {
#ifdef GM_BUILTIN_RENDERER
  // Fallback to regular rect if not implemented in software renderer yet
  gmsc_fill_rect(&_gm_screen, c, (int)x, (int)y, (int)w, (int)h);
  return 0;
#elif !defined(GM_NO_GAPI_DRAW)
  return gapi_draw_rounded_rect(x, y, w, h, r, c);
#else
  return -1;
#endif
}
#define gm_draw_rounded_rectangle(x, y, w, h, r, c)                            \
  gm_draw_roundrect(x, y, w, h, r, c)

/**
 * @brief Draws a circle.
 */
static inline int32_t gm_draw_circle(double center_x, double center_y,
                                     double radius, gmColor c) {
#ifdef GM_BUILTIN_RENDERER
  gmsc_fill_circle(&_gm_screen, c, (int)center_x, (int)center_y, (int)radius);
  return 0;
#elif !defined(GM_NO_GAPI_DRAW)
  return gapi_draw_circle(center_x, center_y, radius, c);
#else
  return -1;
#endif
}

static inline int32_t gm_circle(gmCirc c, gmColor color) {
  return gm_draw_circle(c.pos.x, c.pos.y, c.r, color);
}

/**
 * @brief Draws an ellipse centered at a point.
 */
static inline int32_t gm_draw_ellipse(double x, double y, double w, double h,
                                      gmColor c) {
#ifdef GM_BUILTIN_RENDERER
  // Fallback to circle or rect for now
  gmsc_fill_circle(&_gm_screen, c, (int)x, (int)y, (int)(w > h ? w : h) / 2);
  return 0;
#elif !defined(GM_NO_GAPI_DRAW)
  return gapi_draw_ellipse(x, y, w, h, c);
#else
  return -1;
#endif
}

/**
 * @brief Draws a triangle.
 */
static inline int32_t gm_draw_triangle(double x1, double y1, double x2,
                                       double y2, double x3, double y3,
                                       gmColor c) {
#ifdef GM_BUILTIN_RENDERER
  gmsc_fill_triangle(&_gm_screen, c, (int)x1, (int)y1, (int)x2, (int)y2, (int)x3,
                     (int)y3);
  return 0;
#elif !defined(GM_NO_GAPI_DRAW)
  return gapi_draw_triangle(x1, y1, x2, y2, x3, y3, c);
#else
  return -1;
#endif
}

/**
 * @brief Draws an image centered at a point.
 */
static inline int32_t gm_draw_image(gmImage img, double x, double y, double w,
                                    double h) {
#ifdef GM_BUILTIN_RENDERER
  // Software image drawing not yet implemented in renderer.h
  return -1;
#elif !defined(GM_NO_GAPI_DRAW)
  return gapi_draw_image(img.handle, x, y, w, h);
#else
  return -1;
#endif
}

/**
 * @brief Draws text centered at a point.
 */
static inline int32_t gm_draw_text(double x, double y, const char *text,
                                   const char *font, double font_size,
                                   gmColor c) {
#ifdef GM_BUILTIN_RENDERER
  // Software text drawing not yet implemented
  return -1;
#elif !defined(GM_NO_GAPI_DRAW)
  return gapi_draw_text(x, y, font_size, text, font, 0, c);
#else
  return -1;
#endif
}

// ---------------------------------------------------------------------------
// ------------------------- Object-Based Helpers ----------------------------
// ---------------------------------------------------------------------------

/**
 * @brief Draws a physics body based on its collider type.
 *
 * This function checks the body's collider type and calls the appropriate
 * drawing function (e.g., gm_draw_rectangle for GM_COLLIDER_RECT).
 *
 * @param body A pointer to the body to draw.
 * @param c The color to draw the body.
 */
void gm_draw_body(const gmBody *body, gmColor c) {
  if (body == NULL || !body->is_active) {
    return;
  }
  switch (body->collider_type) {
  case GM_COLLIDER_RECT:
    gm_draw_rectangle(body->position.x, body->position.y, body->width,
                      body->height, c);
    break;
  case GM_COLLIDER_CIRCLE:
    gm_draw_circle(body->position.x, body->position.y, body->radius, c);
    break;
  }
}

/**
 * @brief Draws a rectangular physics body.
 * @param body A pointer to the body to draw.
 * @param c The color to draw the body.
 */
void gm_draw_rect_body(const gmBody *body, gmColor c) {
  gm_draw_rectangle(body->position.x, body->position.y, body->width,
                    body->height, c);
}

/**
 * @brief Draws an array of rectangular physics bodies.
 * @param bodies A pointer to the array of bodies.
 * @param number The number of bodies in the array.
 * @param c The color to draw the bodies.
 */
void gm_draw_rect_bodies(const gmBody *bodies, size_t number, gmColor c) {
  for (size_t i = 0; i < number; i++)
    gm_draw_rect_body(&bodies[i], c);
}

/**
 * @brief Draws a rectangular physics body with rounded corners.
 * @param body A pointer to the body to draw.
 * @param radius The corner radius.
 * @param c The color to draw the body.
 */
void gm_draw_round_rect_body(const gmBody *body, double radius, gmColor c) {
  gm_draw_rounded_rectangle(body->position.x, body->position.y, body->width,
                            body->height, radius, c);
}

/**
 * @brief Draws a circular physics body.
 *
 * If the body is not a circle collider, it approximates the radius.
 *
 * @param body A pointer to the body to draw.
 * @param c The color to draw the body.
 */
void gm_draw_circle_body(const gmBody *body, gmColor c) {
  double radius = body->collider_type == GM_COLLIDER_CIRCLE
                      ? body->radius
                      : (body->width + body->height) / 4.0;
  gm_draw_circle(body->position.x, body->position.y, radius, c);
}

/**
 * @brief Draws an array of circular physics bodies.
 * @param bodies A pointer to the array of bodies.
 * @param number The number of bodies in the array.
 * @param c The color to draw the bodies.
 */
void gm_draw_circle_bodies(const gmBody *bodies, size_t number, gmColor c) {
  for (size_t i = 0; i < number; i++)
    gm_draw_circle_body(&bodies[i], c);
}

/**
 * @brief Draws an elliptical physics body.
 * @param body A pointer to the body to draw.
 * @param c The color to draw the body.
 */
void gm_draw_ellipse_body(const gmBody *body, gmColor c) {
  if (body == NULL || !body->is_active)
    return;
  gm_draw_ellipse(body->position.x, body->position.y, body->width, body->height,
                  c);
}

/**
 * @brief Draws an array of elliptical physics bodies.
 * @param bodies A pointer to the array of bodies.
 * @param number The number of bodies in the array.
 * @param c The color to draw the bodies.
 */
void gm_draw_ellipse_bodies(const gmBody *bodies, size_t number, gmColor c) {
  for (size_t i = 0; i < number; i++)
    gm_draw_ellipse_body(&bodies[i], c);
}

/**
 * @brief Draws a triangular physics body.
 *
 * The body's position is the first vertex. The other two vertices are
 * specified as offsets from the first.
 *
 * @param body A pointer to the body to draw.
 * @param x2_offset The x-offset of the second vertex.
 * @param y2_offset The y-offset of the second vertex.
 * @param x3_offset The x-offset of the third vertex.
 * @param y3_offset The y-offset of the third vertex.
 * @param c The color to draw the body.
 */
void gm_draw_triangle_body(const gmBody *body, double x2_offset,
                           double y2_offset, double x3_offset, double y3_offset,
                           gmColor c) {
  if (body == NULL || !body->is_active)
    return;
  gm_draw_triangle(body->position.x, body->position.y,
                   body->position.x + x2_offset, body->position.y + y2_offset,
                   body->position.x + x3_offset, body->position.y + y3_offset,
                   c);
}

/**
 * @brief Draws an array of triangular physics bodies.
 * @param bodies A pointer to the array of bodies.
 * @param number The number of bodies in the array.
 * @param x2_offset The x-offset of the second vertex for all triangles.
 * @param y2_offset The y-offset of the second vertex for all triangles.
 * @param x3_offset The x-offset of the third vertex for all triangles.
 * @param y3_offset The y-offset of the third vertex for all triangles.
 * @param c The color to draw the bodies.
 */
void gm_draw_triangle_bodies(const gmBody *bodies, size_t number,
                             double x2_offset, double y2_offset,
                             double x3_offset, double y3_offset, gmColor c) {
  for (size_t i = 0; i < number; i++)
    gm_draw_triangle_body(&bodies[i], x2_offset, y2_offset, x3_offset,
                          y3_offset, c);
}

/**
 * @brief Draws an image at a physics body's position.
 * @param body A pointer to the body.
 * @param img The image to draw.
 */
void gm_draw_image_body(const gmBody *body, gmImage img) {
  if (body == NULL || !body->is_active)
    return;
  gm_draw_image(img, body->position.x, body->position.y, body->width,
                body->height);
}

/**
 * @brief Draws the same image for an array of physics bodies.
 * @param bodies A pointer to the array of bodies.
 * @param number The number of bodies in the array.
 * @param img The image to draw.
 */
void gm_draw_image_bodies(const gmBody *bodies, size_t number, gmImage img) {
  for (size_t i = 0; i < number; i++)
    gm_draw_image_body(&bodies[i], img);
}

/**
 * @brief Draws text at a physics body's position.
 * @param body A pointer to the body.
 * @param text The null-terminated string to draw.
 * @param font_size The size of the font.
 * @param c The color of the text.
 */
void gm_draw_text_body(const gmBody *body, const char *text, const char *font,
                       double font_size, gmColor c) {
  if (body == NULL || !body->is_active)
    return;
  gm_draw_text(body->position.x, body->position.y, text, font, font_size, c);
}

/**
 * @brief Draws the same text for an array of physics bodies.
 * @param bodies A pointer to the array of bodies.
 * @param number The number of bodies in the array.
 * @param text The null-terminated string to draw.
 * @param font_size The size of the font.
 * @param c The color of the text.
 */
void gm_draw_text_bodies(const gmBody *bodies, size_t number, const char *text,
                         const char *font, double font_size, gmColor c) {
  for (size_t i = 0; i < number; i++)
    gm_draw_text_body(&bodies[i], text, font, font_size, c);
}

#endif // GM_DRAW_H_INCLUDED
