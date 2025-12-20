# Scale


To draw a simple scale you use `gmw_scale`, which returns
if the scale is currently pressed(and maybe moving).

you can make the scale animated by using [`gmw_scale_anim`](/reference/scale_8h.html#a6e11a61188aef49b7e1705da260b748c),
and passing an additional
double pointer argument it will use for animations.
The value of the scale is between 0 and 1, so you will have to convert and round to
your own scale.

Gama automatically guesses if it is a horizontal or vertical scale by looking at it's
dimensions.

## Examples

```c
int age = 0;
double scale_value = age / 100;
double scale_value_anim;

do {
    if(gmw_scale(0, 0, 0.5, 0.1, &scale_value)) {
        age = scale_value * 100; // update the age
    }
    // animated scale
    gmw_scale_anim(0, 0.3, 0.5, 0.1, &scale_value, &scale_value_anim);
} ...;
```


## Reference

See reference at [scale.h reference](/reference/scale_8h.html)
