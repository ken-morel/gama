#include <gama.h>

int main() {
  gama_init();
  double theta = 0.0;
  for (; gama_isrunning(); theta = gama_frame()) {
  }
  return 0;
}
