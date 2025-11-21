#include <stdio.h>
#include <gama.h>

int main() {
  gapi_init(800, 600, "Gama Window"); // Call gapi_init with window parameters
  int count = 0;

  for (double dt = 0.0; gapi_yield(&dt);count++) {
    // Game loop continues as long as the window is open
    // dt contains the delta time since last frame
  }
  printf("Bye\n");
  return 0;
}
