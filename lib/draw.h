#pragma once

#include "color.h"
#include "gapi.h"
#include "body.h" // Include the body for our new functions
#include "physics.h" // Include physics for the render function
#include <stdint.h>

// ---------------------------------------------------------------------------
// ------------------------- Immediate-Mode Primitives -----------------------
// ---------------------------------------------------------------------------

int32_t gama_draw_raw_rectangle(double x, double y, double w, double h, gama_color c) {
  return gapi_draw_rect(x, y, w, h, gama_color_red(c), gama_color_green(c),
                        gama_color_blue(c), gama_color_alpha(c));
}
int32_t gama_draw_raw_rounded_rectangle(double x, double y, double w, double h,
                                double r, gama_color c) {
  return gapi_draw_rounded_rect(x, y, w, h, r, gama_color_red(c),
                                gama_color_green(c), gama_color_blue(c),
                                gama_color_alpha(c));
}

int32_t gama_draw_raw_circle(double center_x, double center_y, double radius, gama_color c) {
  return gapi_draw_circle(center_x, center_y, radius, gama_color_red(c),
                          gama_color_green(c), gama_color_blue(c),
                          gama_color_alpha(c));
}

int32_t gama_draw_raw_ellipse(double x, double y, double w, double h, gama_color c) {
  return gapi_draw_ellipse(x, y, w, h, gama_color_red(c), gama_color_green(c),
                           gama_color_blue(c), gama_color_alpha(c));
}

int32_t gama_draw_raw_triangle(double x1, double y1, double x2, double y2, double x3,
                       double y3, gama_color c) {
  return gapi_draw_triangle(x1, y1, x2, y2, x3, y3, gama_color_red(c),
                            gama_color_green(c), gama_color_blue(c),
                            gama_color_alpha(c));
}


// ---------------------------------------------------------------------------
// ------------------------- Object-Based Helpers ----------------------------
// ---------------------------------------------------------------------------

// Draws a physics body based on its collider type
void gama_draw_body(const gama_body *body, gama_color c) {
    if (body == NULL || !body->is_active) {
        return;
    }
    switch (body->collider_type) {
        case GAMA_COLLIDER_RECT:
            gama_draw_raw_rectangle(body->position.x, body->position.y, body->width, body->height, c);
            break;
        case GAMA_COLLIDER_CIRCLE:
            gama_draw_raw_circle(body->position.x, body->position.y, body->radius, c);
            break;
    }
}

// Updates physics and then draws a body
void gama_render_body(gama_body *body, double dt, gama_color c) {
    if (body == NULL) {
        return;
    }
    gama_body_update(body, dt);
    gama_draw_body(body, c);
}