#include <GLES3/gl3.h>
#include <emscripten.h>

#include "../app.h"

App *gama;

void main_loop() {
  _gama_update(gama);
  glClear(GL_COLOR_BUFFER_BIT);
  _gama_render(gama);
}

int main() {
  gama = GamaCreateApp();
  _gama_init(gama);
  _gama_create(gama);
  emscripten_set_main_loop(main_loop, 0, 1);
  _gama_shutdown(gama);
  return 0;
}
