#include <gama.h>
#include <stdio.h>

int main() {
  printf("Starting Gama Demo\n");

  gama_init(800, 600, "Bouncing Ball Demo");

  gama_body floor = gama_body_create(0, -0.9, 1.8, 0.2, GAMA_COLLIDER_RECT);
  floor.mass = 0;          // Mass of 0 makes it static
  floor.restitution = 0.5; // Slightly bouncy
  // Ball (Dynamic Circle)
  gama_body ball = gama_body_create(
      0, 0.5, 0.2, 0.2,
      GAMA_COLLIDER_CIRCLE); // w,h act as radius for circle create
  ball.restitution = 0.7;    // Bouncier than floor
  ball.acceleration.y = -5;

  while (gama_yield()) {
    gama_physics_update2(&floor, &ball);

    gama_draw_rect_body(&floor, LIGHTGREEN);

    gama_draw_circle_body(&ball, BLUE);

    gama_draw_text(0, 0.9, "Gama Physics Demo", 0.5, WHITE);
  }

  printf("Gama Demo Finished. Bye!\n");
  return 0;
}
