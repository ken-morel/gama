#include <gama.h>

int main() {
  gama_init(800, 600, "Bouncing Ball Demo");

  gama_body floor = gama_rectangle_create(0, -0.9, 1.8, 0.2);
  floor.mass = 0; // Mass of 0 makes it static

  gama_body ball =
      gama_circle_create(0, 0.5, 0.2); // w,h act as radius for circle create
  ball.restitution = 0.7;              // Bouncy than floor
  ball.acceleration.y = -5;

  while (gama_yield()) {
    gama_physics_update2(&floor, &ball);

    gama_draw_rect_body(&floor, LIGHTGREEN);
    gama_draw_circle_body(&ball, BLUE);

    gama_draw_text(0, 0.9, "Gama Physics Demo", 0.5, WHITE);

    if (gama_key('R'))
      ball.velocity.x = 0.1;
    else if (gama_key('L'))
      ball.velocity.x = -0.1;
  }
  return 0;
}
