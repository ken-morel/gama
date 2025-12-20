# Using gama animation functions

To help you add animations, gama provide a few functions
you animate doubles over time.

You can view the [animate.h reference](/reference/animate_8h.html)

## Examples

```c
double x = 0.5;

do {
  gm_animate_spring(&x, gm_key('R') ? 1 : -1, 1);
  gm_draw_circle(x, 0, 0.2, GM_WHITE);
} while(gm_yield());
```
