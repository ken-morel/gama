#include "body.h"
#include "color.h"
#include <gama.h>

int main() {
  gama_init(600, 600, "Select File Ball Demo");

  gama_color ball_color;
  gama_body ball = gama_circle_create(0, 0.5, 0.2);
  ball.restitution = 0.8; // Bouncy
  ball.acceleration.y = -0.2;

  gama_body floor = gama_rectangle_create(0, 0, 1.8, 0.1);
  floor.mass = 100; // Mass of 0 makes it static
  floor.restitution = 0.8;

  gama_body second_floor = gama_rectangle_create(0, -0.3, 1.8, 0.1);
  second_floor.mass = 100;
  second_floor.restitution = 0.8;

  gama_body third_floor = gama_rectangle_create(0, -1, 1.8, 0.1);
  third_floor.mass = 0;

  while (gama_yield()) {
    gama_physics_update4(&ball, &floor, &second_floor, &third_floor);

    gama_draw_rect_body(&floor, DARKGREEN);
    gama_draw_rect_body(&second_floor, BLUE);
    gama_draw_rect_body(&third_floor, DARKBLUE);
    gama_draw_circle_body(&ball, ball_color);

    gama_draw_text(0, 0.9, "Gama Physics Demo", 0.5, WHITE);

    if (gama_hovered(&ball))
      ball_color += BLUE;
    else
      ball_color = GREENYELLOW;

    if (gama_key('R'))
      ball.position.x += gama_dt;
    else if (gama_key('L'))
      ball.position.x += -gama_dt;
    else if (gama_key('U'))
      ball.velocity.y = 1;
  }
  return 0;
}
