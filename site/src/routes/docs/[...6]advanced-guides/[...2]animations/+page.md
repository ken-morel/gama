# Using gama animation functions

Gama provide a few set of functions which help you gradually modify
a value as to animate it over time. It support few easing functions
and methods of operations.

You can view the <a href="/reference/animate_8h.html">animate.h reference</a>

## Examples

```c
double x = 0.5;

do {
  gm_animate_spring(&x, gm_key('R') ? 1 : -1, 1);
  gm_draw_circle(x, 0, 0.2, GM_WHITE);
} while(gm_yield());
```
