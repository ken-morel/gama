#include <gama.h>

int main() {
  gm_init(500, 500, "Gama test application");
  gmSystem sys = gm_system_create();

  gmBody ball_body = gm_circle_body(100, 0, 0, 0.2);
  ball_body.velocity.y = 0.4;
  ball_body.velocity.x = 0.4;
  gm_system_push(&sys, &ball_body);

  gmBody walls[2] = {
      gm_rectangle_body(0, 0, -1, 2, 0),
      gm_rectangle_body(0, 0, 1, 2, 0),
  };
  gm_system_push_array(&sys, 2, walls);

  gmBody paddles[2] = {
      gm_rectangle_body(0, -1, 0, 0.2, 0.7),
      gm_rectangle_body(0, 1, 0, 0.2, 0.7),
  };
  gm_system_push_array(&sys, 2, paddles);

  do {
    double dt = gm_dt();
    gm_system_update(&sys);

    gm_draw_circle_body(&ball_body, GM_BISQUE);
    gm_draw_rect_bodies(paddles, 2, GM_DARKGOLDENROD);

    double velocity = gm_key('U') ? -4 : gm_key('D') ? 4 : 0;
    paddles[0].velocity.y = velocity;
    paddles[1].velocity.y = velocity;

  } while (gm_yield());
  return 0;
}
