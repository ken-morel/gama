#include "draw.h"
#include <gama.h>
#include <stdio.h>

int main() {
  gama_init(800, 600, "Gama Window");
  double x = 0;
  double dt = 0;

  while (gama_yield(&dt)) {
    gama_draw_rounded_rectangle(x, 0, 1, 1, 0.02, GREEN);
    if (gama_key_shortcut_pressed('L')) {
      x -= 0.1 * dt;
    } else if (gama_key_shortcut_pressed('R')) {
      x += 0.1 * dt;
    }
  }
  return 0;
}
