#include <gama.h>

int gameOver = 0;

int main() {
  gama_init();
  GamaText hello = gama_text("Hello world");
  gama_set_velocity(hello, 0, 0.1);
  for (double theta = 0.0; !gameOver; theta = gama_yield()) {
    gama_draw(hello);
    if(gama_clicked(hello))
      gameOver = 1;
  }
  gama_destroy(hello);
  return 0;
}
