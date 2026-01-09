#include "gama/draw.h"
#include <gama.h>

int main() {
  gm_init(0, 0, "Hello world");

  gm_background(0x333333FF);

  printf("printing some text here");
  do {
    gm_draw_rectangle(0, 1, 1, 1, GM_BLACK);
  } while (gm_yield());
  return 0;
}
