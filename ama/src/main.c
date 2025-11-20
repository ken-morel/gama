#include <gama.h>

int gameOver = 0;

int main() {
  gama_init();
  for (double theta = 0.0; !gameOver; theta = gama_yield()) {
  }
  return 0;
}
