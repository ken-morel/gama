#include <gama.h>

int main() {
  gm_init(500, 500, "skeleton gama application");
  // create shapes
  do {
    gm_draw_circle(0, 0, 1, GM_BLUE);
  } while (gm_yield());
  // destroy shapes
  return 0;
}
