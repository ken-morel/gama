/**
 * @file draw.h
 * @brief Functions for drawing shapes, text, and images.
 *
 * This file provides a set of functions for immediate-mode rendering of
 * various primitives, as well as helper functions to draw physics bodies
 * (`gmBody`). All coordinates are in world space.
 */

#pragma once

#include "body.h"
#include "color.h"
#include "gapi.h"
#include "image.h" // For gmImage
#include <stdint.h>

// ---------------------------------------------------------------------------
// ------------------------- Immediate-Mode Primitives -----------------------
// ---------------------------------------------------------------------------

/**
 * @brief Draws a line segment.
 * @param x1 The x-coordinate of the starting point.
 * @param y1 The y-coordinate of the starting point.
 * @param x2 The x-coordinate of the ending point.
 * @param y2 The y-coordinate of the ending point.
 * @param thickness The thickness of the line in pixels.
 * @param c The color of the line.
 * @return An identifier for the drawing command.
 */
int32_t gm_draw_line(double x1, double y1, double x2, double y2,
                     double thickness, gmColor c) {
  return gapi_draw_line(x1, y1, x2, y2, thickness, gm_red(c), gm_green(c),
                        gm_blue(c), gm_alpha(c));
}

/**
 * @brief Draws a rectangle.
 * @param x The x-coordinate of the top-left corner.
 * @param y The y-coordinate of the top-left corner.
 * @param w The width of the rectangle.
 * @param h The height of the rectangle.
 * @param c The color of the rectangle.
 * @return An identifier for the drawing command.
 */
int32_t gm_draw_rectangle(double x, double y, double w, double h, gmColor c) {
  return gapi_draw_rect(x, y, w, h, gm_red(c), gm_green(c), gm_blue(c),
                        gm_alpha(c));
}

/**
 * @brief Draws a rectangle with rounded corners.
 * @param x The x-coordinate of the top-left corner.
 * @param y The y-coordinate of the top-left corner.
 * @param w The width of the rectangle.
 * @param h The height of the rectangle.
 * @param r The corner radius.
 * @param c The color of the rectangle.
 * @return An identifier for the drawing command.
 */
int32_t gm_draw_rounded_rectangle(double x, double y, double w, double h,
                                  double r, gmColor c) {
  return gapi_draw_rounded_rect(x, y, w, h, r, gm_red(c), gm_green(c),
                                gm_blue(c), gm_alpha(c));
}

/**
 * @brief Draws a circle.
 * @param center_x The x-coordinate of the center of the circle.
 * @param center_y The y-coordinate of the center of the circle.
 * @param radius The radius of the circle.
 * @param c The color of the circle.
 * @return An identifier for the drawing command.
 */
int32_t gm_draw_circle(double center_x, double center_y, double radius,
                       gmColor c) {
  return gapi_draw_circle(center_x, center_y, radius, gm_red(c), gm_green(c),
                          gm_blue(c), gm_alpha(c));
}

/**
 * @brief Draws an ellipse.
 * @param x The x-coordinate of the top-left corner of the bounding box.
 * @param y The y-coordinate of the top-left corner of the bounding box.
 * @param w The width of the ellipse.
 * @param h The height of the ellipse.
 * @param c The color of the ellipse.
 * @return An identifier for the drawing command.
 */
int32_t gm_draw_ellipse(double x, double y, double w, double h, gmColor c) {
  return gapi_draw_ellipse(x, y, w, h, gm_red(c), gm_green(c), gm_blue(c),
                           gm_alpha(c));
}

/**
 * @brief Draws a triangle.
 * @param x1 The x-coordinate of the first vertex.
 * @param y1 The y-coordinate of the first vertex.
 * @param x2 The x-coordinate of the second vertex.
 * @param y2 The y-coordinate of the second vertex.
 * @param x3 The x-coordinate of the third vertex.
 * @param y3 The y-coordinate of the third vertex.
 * @param c The color of the triangle.
 * @return An identifier for the drawing command.
 */
int32_t gm_draw_triangle(double x1, double y1, double x2, double y2, double x3,
                         double y3, gmColor c) {
  return gapi_draw_triangle(x1, y1, x2, y2, x3, y3, gm_red(c), gm_green(c),
                            gm_blue(c), gm_alpha(c));
}

/**
 * @brief Draws an image.
 * @param img The image to draw.
 * @param x The x-coordinate of the top-left corner.
 * @param y The y-coordinate of the top-left corner.
 * @param w The width to draw the image.
 * @param h The height to draw the image.
 * @return An identifier for the drawing command.
 */
int32_t gm_draw_image(gmImage img, double x, double y, double w, double h) {
  return gapi_draw_image(img.handle, x, y, w, h);
}
/**
 * @brief Draws text.
 * @param x The x-coordinate for the text position.
 * @param y The y-coordinate for the text position.
 * @param text The null-terminated string to draw.
 * @param font The null-terminated font name to use.
 * @param font_size The size of the font.
 * @param c The color of the text.
 * @return An identifier for the drawing command.
 */
int32_t gm_draw_text(double x, double y, const char *text, const char *font,
                     double font_size, gmColor c) {
  return gapi_draw_text(x, y, font_size, text, font, 0, gm_red(c), gm_green(c),
                        gm_blue(c), gm_alpha(c));
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
