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
  }
}
```

you use `gm_draw` functions to draw to the canvas, 
when you call a draw function, it may not automatically draw to the screen because
the drawing api may not be ready to draw yet,
in that case it will add your draw call to a list of draw calls and call them
only when the window is ready to be drawed on.

## The coordinate system

To draw shapes you need obviously to indicate gama where to draw them, using specific
coordinates, when you draw on gama you use a graph paper like coordinates where
the x and y axae range from -1 to 1, but the x or y ranges may exceed that if your window 
is not completely square causing some parts to be hidden.

![Gridlines](/lineup-gridlines.png)

Thus a rectangle of width 2 occupies the whole square window.

## Reference

You can find other draw functions in the [draw.h reference](/reference/draw_8h.html)
