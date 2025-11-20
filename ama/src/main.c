#include <stdio.h>
#include <gama.h>

int main() {
  printf("Starting\n");
  gama_init();
  printf("Initialized gama\n");
  int n = 0;
  for (double theta = 0.0; gama_runs(); theta = gama_yield()) {
    n++;
    if (n > 10) {
      printf("Quiting");
      gama_quit();
    }
    printf("Yielding\n");
  }
  printf("Bye");
  return 0;
}
