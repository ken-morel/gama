# Drawing shapes

Gama is an imediate mode library, what does that mean? simply 
that you draw the shapes directly, and you don't need to 
create `shape objects or shape structs`.


```c
#include <gama.h>

int main() {
  gm_init(500, 500, "Drawing shapes");

  do {
    gm_draw_rectangle(0, 0, 1, 1, GM_BLUE);
    gm
  }
}
```

you use `gm_draw` functions to draw to the window,
