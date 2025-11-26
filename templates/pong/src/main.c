#include "body.h"
#include "system.h"
#include <gama.h>

int main() {
  gm_init(500, 500, "Gama test application");
  // creating a system
  gmSystem sys = gm_system_create();

  // a litle ball of 10 grams
  gmBody ball_body = gm_circle_body(100, 0, 0, 0.2);
  gm_system_push(&sys, &ball_body);

  gmBody paddles[2] = {
      gm_rectangle_body(0, -1, 0, 0.1, 0.5),
      gm_rectangle_body(0, 1, 0, 0.1, 0.5),
  };
  gm_system_push_array(&sys, 2, paddles);

  do {

    gm_system_update(&sys, 100);

  } while (gm_yield());
  return 0;
}
