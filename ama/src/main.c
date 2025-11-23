#include "color.h"
#include <gama.h>

int main() {
  gama_init(800, 600, "Bouncing Ball Demo");

  gama_body floor = gama_rectangle_create(0.5, -0.9, 1.8, 0.2);
  floor.mass = 0; // Mass of 0 makes it static

  gama_body ball =
      gama_circle_create(0, 0.5, 0.2); // w,h act as radius for circle create
  ball.restitution = 0.8;              // Bouncy than floor
  ball.acceleration.y = -0.2;

  gama_color color;
  gama_color fcolor;

  while (gama_yield()) {
    gama_physics_update2(&floor, &ball);

    gama_draw_rect_body(&floor, fcolor);
    gama_draw_circle_body(&ball, color);

    gama_draw_text(0, 0.9, "Gama Physics Demo", 0.5, WHITE);

    if (gama_hovered(&ball))
      color = RED;
    else
      color = GREENYELLOW;



    if (gama_hovered(&floor))
      fcolor = LIGHTBLUE;
    else
      fcolor = LIGHTCORAL;


    if (gama_key('R'))
      ball.position.x += gama_dt;
    else if (gama_key('L'))
      ball.position.x += -gama_dt;
    else if (gama_key('U'))
      ball.velocity.y = -1;
  }
  return 0;
}
