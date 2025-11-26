#pragma once

#include "body.h"
#include "color.h"
#include "gapi.h"
#include "image.h" // For gmImage
#include <stdint.h>

// ---------------------------------------------------------------------------
// ------------------------- Immediate-Mode Primitives -----------------------
// ---------------------------------------------------------------------------

int32_t gm_draw_line(double x1, double y1, double x2, double y2,
                       double thickness, gmColor c) {
  return gapi_draw_line(x1, y1, x2, y2, thickness, gm_red(c), gm_green(c),
                        gm_blue(c), gm_alpha(c));
}

int32_t gm_draw_rectangle(double x, double y, double w, double h, gmColor c) {
  return gapi_draw_rect(x, y, w, h, gm_red(c), gm_green(c), gm_blue(c),
                        gm_alpha(c));
}
int32_t gm_draw_rounded_rectangle(double x, double y, double w, double h,
                                    double r, gmColor c) {
  return gapi_draw_rounded_rect(x, y, w, h, r, gm_red(c), gm_green(c),
                                gm_blue(c), gm_alpha(c));
}

int32_t gm_draw_circle(double center_x, double center_y, double radius,
                         gmColor c) {
  return gapi_draw_circle(center_x, center_y, radius, gm_red(c), gm_green(c),
                          gm_blue(c), gm_alpha(c));
}

int32_t gm_draw_ellipse(double x, double y, double w, double h, gmColor c) {
  return gapi_draw_ellipse(x, y, w, h, gm_red(c), gm_green(c), gm_blue(c),
                           gm_alpha(c));
}

int32_t gm_draw_triangle(double x1, double y1, double x2, double y2,
                           double x3, double y3, gmColor c) {
  return gapi_draw_triangle(x1, y1, x2, y2, x3, y3, gm_red(c), gm_green(c),
                            gm_blue(c), gm_alpha(c));
}

int32_t gm_draw_image(gmImage img, double x, double y, double w,
                        double h) {
  return gapi_draw_image(img.handle, x, y, w, h);
}

char *gm_text_font = "Monospace";
char gm_text_align = 'c';

int32_t gm_draw_text(double x, double y, const char *text, double font_size,
                       gmColor c) {
  // Assuming a default font 'm' for monospace for now.
  // Style and align are defaulted for simplicity.
  return gapi_draw_text(
      x, y, font_size, text, gm_text_font, 0,
      gm_text_align, // maxwidth and height params are weird in gapi, let's
                       // keep it simple for now
      gm_red(c), gm_green(c), gm_blue(c), gm_alpha(c));
}

// ---------------------------------------------------------------------------
// ------------------------- Object-Based Helpers ----------------------------
// ---------------------------------------------------------------------------

// A generic dispatcher for drawing a physics body based on its collider type
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

// Specialized drawing functions for physics bodies
void gm_draw_rect_body(const gmBody *body, gmColor c) {
  gm_draw_rectangle(body->position.x, body->position.y, body->width,
                      body->height, c);
}
void gm_draw_rect_bodies(const gmBody *bodies, size_t number, gmColor c) {
  for (size_t i = 0; i < number; i++)
    gm_draw_rect_body(&bodies[i], c);
}

void gm_draw_round_rect_body(const gmBody *body, double radius,
                               gmColor c) {
  gm_draw_rounded_rectangle(body->position.x, body->position.y, body->width,
                              body->height, radius, c);
}


void gm_draw_circle_body(const gmBody *body, gmColor c) {
  double radius = body->collider_type == GM_COLLIDER_CIRCLE
                      ? body->radius
                      : (body->width + body->height) / 4.0;
  gm_draw_circle(body->position.x, body->position.y, radius, c);
}

void gm_draw_circle_bodies(const gmBody *bodies, size_t number, gmColor c) {
  for (size_t i = 0; i < number; i++)
    gm_draw_circle_body(&bodies[i], c);
}

void gm_draw_ellipse_body(const gmBody *body, gmColor c) {
  if (body == NULL || !body->is_active)
    return;
  gm_draw_ellipse(body->position.x, body->position.y, body->width,
                    body->height, c);
}

void gm_draw_ellipse_bodies(const gmBody *bodies, size_t number, gmColor c) {
  for (size_t i = 0; i < number; i++)
    gm_draw_ellipse_body(&bodies[i], c);
}
void gm_draw_triangle_body(const gmBody *body, double x2_offset,
                             double y2_offset, double x3_offset,
                             double y3_offset, gmColor c) {
  if (body == NULL || !body->is_active)
    return;
  gm_draw_triangle(body->position.x, body->position.y,
                     body->position.x + x2_offset, body->position.y + y2_offset,
                     body->position.x + x3_offset, body->position.y + y3_offset,
                     c);

}

void gm_draw_triangle_bodies(const gmBody *bodies, size_t number, double x2_offset,
                             double y2_offset, double x3_offset,
                             double y3_offset, gmColor c) {
  for (size_t i = 0; i < number; i++)
    gm_draw_triangle_body(&bodies[i], x2_offset, y2_offset, x3_offset, y3_offset, c);
}
void gm_draw_image_body(const gmBody *body, gmImage img) {
  if (body == NULL || !body->is_active)
    return;
  gm_draw_image(img, body->position.x, body->position.y, body->width,
                  body->height);
}

void gm_draw_image_bodies(const gmBody *bodies, size_t number, gmImage img) {
  for (size_t i = 0; i < number; i++)
    gm_draw_image_body(&bodies[i], img);
}
void gm_draw_text_body(const gmBody *body, const char *text,
                         double font_size, gmColor c) {
  if (body == NULL || !body->is_active)
    return;
  gm_draw_text(body->position.x, body->position.y, text, font_size, c);
}

void gm_draw_text_bodies(const gmBody *bodies, size_t number,  const char *text,
                         double font_size, gmColor c) {
  for (size_t i = 0; i < number; i++)
    gm_draw_text_body(&bodies[i], text, font_size, c);
}
