#include <stdio.h>
#include <gama.h>

int main() {
  gapi_init(800, 600, "Gama Window");
  struct gama_rectangle r = gama_rectangle_create(0, 0, 1, 1, BLUE);
  r.velocity.y = 0.002;

  for (double dt = 0.0; gapi_yield(&dt);) {
    printf("C draws\n");
    gama_rectangle_render(&r, dt);
    printf("c finished\n");
  }
  printf("Bye\n");
  return 0;
}
