#include <gama.h>

int main() {
  gm_init(500, 500, "My gama game");
  gmSystem sys = gm_system_create(); // a physics system
  sys.damping = 0.3;

  gmBody circles[10];
  gmBody rects[5];

  for (size_t i = 0; i < 10; i++) {
    circles[i] =
        gm_circle_body(i % 2 == 0 ? 10 : 50, 1 - (double)i / 5, (double)i / 5,
                       i % 2 == 0 ? 0.1 : 0.15); // create the body of a circle
    circles[i].restitution = 0.8;
    gm_system_push(&sys, &circles[i]); // add the circle to the system
  }
  for (size_t i = 0; i < 5; i++) {
    rects[i] = gm_rectangle_body(20, 1 - (double)i / 2.5, -0.4, 0.2, 0.2);
    rects[i].restitution = 0.8;
    gm_system_push(&sys, &rects[i]);
  }

  // the ground, mass of 0 make it static
  gmBody floor = gm_rectangle_body(0, 0, -1, 2, 0.2);
  gm_system_push(&sys, &floor);

  // left and right walls
  gmBody walls[2] = {
      gm_rectangle_body(0, -1, 0, 0.2, 2),
      gm_rectangle_body(0, 1, 0, 0.2, 2),
  };
  gm_system_push(&sys, &walls[0]);
  gm_system_push(&sys, &walls[1]);

  do {
    gm_dt();
    gm_system_update(&sys, 500);
    for (size_t i = 0; i < 10; i++)
      gm_draw_circle_body(&circles[i], i % 2 == 0 ? GM_BLUE : GM_DARKBLUE);
    for (size_t i = 0; i < 5; i++)
      gm_draw_rect_body(&rects[i], GM_CYAN);
    gm_draw_rect_body(&floor, GM_GREEN);
    gm_draw_rect_body(&walls[0], GM_GREEN);
    gm_draw_rect_body(&walls[1], GM_GREEN);
    if (gm_key(' '))
      sys.acceleration.y = 0.2;
    else
      sys.acceleration.y = -0.3;
  } while (gm_yield());
  return 0;
}
