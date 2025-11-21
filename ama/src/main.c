#include <stdio.h>
#include <gama.h>

int main() {
  gapi_init(800, 600, "Gama Window");
  int count = 0;

  for (double dt = 0.0; gapi_yield(&dt);count++) {
  }
  printf("Bye\n");
  return 0;
}
