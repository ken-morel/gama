#include "draw.h"
#include <gama.h>

int main() {
  gama_init(800, 600, "Gama Window");
  double x = 0;
  double dt = 0;

  while (gama_yield(&dt)) {
    gama_draw_rounded_rectangle(x, 0, 1, 1, 0.02, GREEN);
    if (gama_key('L'))
      x -= 0.1;
    else if (gama_key('R'))
      x += 0.1;
  }
  return 0;
}
