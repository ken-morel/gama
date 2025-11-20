#include <stdio.h>
#include <gama.h>

int main() {
  gapi_init(800, 600, "Gama Window"); // Call gapi_init with window parameters

  for (double dt = 0.0; gapi_yield(&dt);) {
    // Game loop continues as long as the window is open
    // dt contains the delta time since last frame
  }
  printf("Bye\n");
  return 0;
}
