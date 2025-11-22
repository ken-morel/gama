#include <stdio.h>
#include <gama.h>

int main() {
  printf("Starting Gama Demo\n");

  gama_init(800, 600, "Bouncing Ball Demo");

  // --- Create Physics Bodies ---
  
  // Floor/Platform (Static Rectangle)
  gama_body floor = gama_body_create(0, -0.9, 1.8, 0.2, GAMA_COLLIDER_RECT);
  floor.is_active = 1;
  floor.mass = 0.0; // Mass of 0 makes it static
  floor.restitution = 0.5; // Slightly bouncy
  floor.friction = 0.8; // Some friction

  // Ball (Dynamic Circle)
  gama_body ball = gama_body_create(0, 0.5, 0.2, 0.2, GAMA_COLLIDER_CIRCLE); // w,h act as radius for circle create
  ball.is_active = 1;
  ball.mass = 1.0; // Dynamic
  ball.restitution = 0.7; // Bouncier than floor
  ball.friction = 0.5;
  ball.acceleration.y = -0.5; // Gravity (adjusted for normalized space and dt)

  gama_body bodies[] = { floor, ball };
  int body_count = 2;
  
  double dt = 0.0; // Delta time will be updated by gama_yield

  while (gama_runs()) {
    // Call gama_yield to update delta time and sync with graphics thread
    // The loop continues as long as gama_yield returns true (1)
    if (!gama_yield(&dt)) {
        break; // Exit loop if window is closed
    }

    // --- Update Physics ---
    gama_physics_update(bodies, body_count, dt);

    // --- Draw Everything ---
    // Clear screen is handled by gapi_yield's begin()
    
    // Draw floor
    gama_draw_rect_body(&floor, LIGHTGREEN);
    
    // Draw ball
    gama_draw_circle_body(&ball, BLUE);

    // Optional: Display some text
    gama_draw_text(0, 0.9, "Gama Physics Demo", 0.05, WHITE);
  }

  printf("Gama Demo Finished. Bye!\n");
  gama_quit();
  return 0;
}