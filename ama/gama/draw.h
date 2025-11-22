#pragma once

#include "color.h"
#include "gapi.h"
#include "body.h"
#include "physics.h"
#include "image.h" // For gama_image
#include <stdint.h>

// ---------------------------------------------------------------------------
// ------------------------- Immediate-Mode Primitives -----------------------
// ---------------------------------------------------------------------------

int32_t gama_draw_line(double x1, double y1, double x2, double y2, double thickness, gama_color c) {
  return gapi_draw_line(x1, y1, x2, y2, thickness, gama_color_red(c), gama_color_green(c),
                        gama_color_blue(c), gama_color_alpha(c));
}

int32_t gama_draw_rectangle(double x, double y, double w, double h, gama_color c) {
  return gapi_draw_rect(x, y, w, h, gama_color_red(c), gama_color_green(c),
                        gama_color_blue(c), gama_color_alpha(c));
}
int32_t gama_draw_rounded_rectangle(double x, double y, double w, double h,
                                double r, gama_color c) {
  return gapi_draw_rounded_rect(x, y, w, h, r, gama_color_red(c),
                                gama_color_green(c), gama_color_blue(c),
                                gama_color_alpha(c));
}

int32_t gama_draw_circle(double center_x, double center_y, double radius, gama_color c) {
  return gapi_draw_circle(center_x, center_y, radius, gama_color_red(c),
                          gama_color_green(c), gama_color_blue(c),
                          gama_color_alpha(c));
}

int32_t gama_draw_ellipse(double x, double y, double w, double h, gama_color c) {
  return gapi_draw_ellipse(x, y, w, h, gama_color_red(c), gama_color_green(c),
                           gama_color_blue(c), gama_color_alpha(c));
}

int32_t gama_draw_triangle(double x1, double y1, double x2, double y2, double x3,
                       double y3, gama_color c) {
  return gapi_draw_triangle(x1, y1, x2, y2, x3, y3, gama_color_red(c),
                            gama_color_green(c), gama_color_blue(c),
                            gama_color_alpha(c));
}

int32_t gama_draw_image(gama_image img, double x, double y, double w, double h) {
    return gapi_draw_image(img.handle, x, y, w, h);
}

char* gama_text_font = "Monospace";
char gama_text_align = 'c';

int32_t gama_draw_text(double x, double y, const char* text, double font_size, gama_color c) {
  // Assuming a default font 'm' for monospace for now.
  // Style and align are defaulted for simplicity.
  return gapi_draw_text(x, y, 1.0, font_size, text, gama_text_font, 0, gama_text_align, // maxwidth and height params are weird in gapi, let's keep it simple for now
                        gama_color_red(c), gama_color_green(c), gama_color_blue(c), gama_color_alpha(c));
}


// ---------------------------------------------------------------------------
// ------------------------- Object-Based Helpers ----------------------------
// ---------------------------------------------------------------------------

// A generic dispatcher for drawing a physics body based on its collider type
void gama_draw_body(const gama_body *body, gama_color c) {
    if (body == NULL || !body->is_active) {
        return;
    }
    switch (body->collider_type) {
        case GAMA_COLLIDER_RECT:
            gama_draw_rectangle(body->position.x, body->position.y, body->width, body->height, c);
            break;
        case GAMA_COLLIDER_CIRCLE:
            gama_draw_circle(body->position.x, body->position.y, body->radius, c);
            break;
    }
}

// Specialized drawing functions for physics bodies
void gama_draw_rect_body(const gama_body *body, gama_color c) {
    if (body == NULL || !body->is_active) return;
    gama_draw_rectangle(body->position.x, body->position.y, body->width, body->height, c);
}

void gama_draw_round_rect_body(const gama_body *body, double radius_factor, gama_color c) {
    if (body == NULL || !body->is_active) return;
    double radius = (body->width < body->height ? body->width : body->height) * radius_factor;
    gama_draw_rounded_rectangle(body->position.x, body->position.y, body->width, body->height, radius, c);
}

void gama_draw_circle_body(const gama_body *body, gama_color c) {
    if (body == NULL || !body->is_active) return;
    double radius = body->collider_type == GAMA_COLLIDER_CIRCLE ? body->radius : (body->width + body->height) / 4.0;
    gama_draw_circle(body->position.x, body->position.y, radius, c);
}

void gama_draw_ellipse_body(const gama_body *body, gama_color c) {
    if (body == NULL || !body->is_active) return;
    gama_draw_ellipse(body->position.x, body->position.y, body->width, body->height, c);
}

void gama_draw_triangle_body(const gama_body *body, double x2_offset, double y2_offset, double x3_offset, double y3_offset, gama_color c) {
    if (body == NULL || !body->is_active) return;
    gama_draw_triangle(body->position.x, body->position.y,
                       body->position.x + x2_offset, body->position.y + y2_offset,
                       body->position.x + x3_offset, body->position.y + y3_offset, c);
}

void gama_draw_image_body(const gama_body *body, gama_image img) {
    if (body == NULL || !body->is_active) return;
    gama_draw_image(img, body->position.x, body->position.y, body->width, body->height);
}

void gama_draw_text_body(const gama_body *body, const char* text, double font_size, gama_color c) {
    if (body == NULL || !body->is_active) return;
    gama_draw_text(body->position.x, body->position.y, text, font_size, c);
}

// Convenience function to update physics and then draw a body generically
void gama_render_body(gama_body *body, double dt, gama_color c) {
    if (body == NULL) {
        return;
    }
    gama_body_update(body, dt); // Physics update
    gama_draw_body(body, c);   // Generic draw
}

// Convenience function to update physics and then draw a body as a rectangle
void gama_render_rect_body(gama_body *body, double dt, gama_color c) {
    if (body == NULL) return;
    gama_body_update(body, dt);
    gama_draw_rect_body(body, c);
}

// Convenience function to update physics and then draw a body as a rounded rectangle
void gama_render_round_rect_body(gama_body *body, double dt, double radius_factor, gama_color c) {
    if (body == NULL) return;
    gama_body_update(body, dt);
    gama_draw_round_rect_body(body, radius_factor, c);
}

// Convenience function to update physics and then draw a body as a circle
void gama_render_circle_body(gama_body *body, double dt, gama_color c) {
    if (body == NULL) return;
    gama_body_update(body, dt);
    gama_draw_circle_body(body, c);
}

// Convenience function to update physics and then draw a body as an ellipse
void gama_render_ellipse_body(gama_body *body, double dt, gama_color c) {
    if (body == NULL) return;
    gama_body_update(body, dt);
    gama_draw_ellipse_body(body, c);
}

// Convenience function to update physics and then draw a body as a triangle
void gama_render_triangle_body(gama_body *body, double dt, double x2_offset, double y2_offset, double x3_offset, double y3_offset, gama_color c) {
    if (body == NULL) return;
    gama_body_update(body, dt);
    gama_draw_triangle_body(body, x2_offset, y2_offset, x3_offset, y3_offset, c);
}

// Convenience function to update physics and then draw a body with an image
void gama_render_image_body(gama_body *body, double dt, gama_image img) {
    if (body == NULL) return;
    gama_body_update(body, dt);
    gama_draw_image_body(body, img);
}

// Convenience function to update physics and then draw a body with text
void gama_render_text_body(gama_body *body, double dt, const char* text, double font_size, gama_color c) {
    if (body == NULL) return;
    gama_body_update(body, dt);
    gama_draw_text_body(body, text, font_size, c);
}
