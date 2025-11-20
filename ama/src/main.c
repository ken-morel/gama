#include <gama.h>

int main() {
  gama_init();
  int n = 0;
  for (double theta = 0.0; gama_runs(); theta = gama_yield()) {
    n++;
    if (n > 100000) {
      gama_quit();
    }
  }
  return 0;
}
