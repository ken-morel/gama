#include "draw.h"
#include <stdio.h>
#include <gama.h>

int main() {
  printf("Starting Gama Demo\n");

  printf("Initializing gama");
  gama_init(800, 600, "Bouncing Ball Demo\0");
  printf("Gama iniialized");

  // --- Create Physics Bodies ---

  printf("Creating body\n");
  // Floor/Platform (Static Rectangle)
  gama_body floor = gama_rectangle_create(0, 0, 1.9, 0.9);
  floor.is_active = 1;
  floor.mass = 0.0; // Mass of 0 makes it static
  floor.restitution = 0.5; // Slightly bouncy
  floor.friction = 0.8; // Some friction

  printf("Creating circle\n");
  // Ball (Dynamic Circle)
  gama_body ball = gama_circle_create(0, 0.9, 0.1); // w,h act as radius for circle create
  ball.is_active = 1;
  ball.mass = 1.0; // Dynamic
  ball.restitution = 0.7; // Bouncier than floor
  ball.friction = 0.5;
  ball.acceleration.y = -0.5; // Gravity (adjusted for normalized space and dt)

  gama_body bodies[] = { floor, ball };
  int body_count = 2;

  // Delta time will be updated by gama_yield
  for(double dt = 0;gama_yield(&dt);) {

    // --- Update Physics ---
    // gama_physics_update(bodies, body_count, dt);

    // --- Draw Everything ---
    // Clear screen is handled by gapi_yield's begin()

    // Draw floor
    // gama_draw_rect_body(&floor, LIGHTGREEN);
    gama_draw_rectangle(0, 1, 0.1, 0.1,GREEN);

    // Draw ball
    // gama_draw_circle_body(&ball, BLUE);

    // Optional: Display some text
    // gama_draw_text(0, 0.9, "Gama Physics Demo", 0.05, WHITE);
  }

  printf("Gama Demo Finished. Bye!\n");
  return 0;
}
