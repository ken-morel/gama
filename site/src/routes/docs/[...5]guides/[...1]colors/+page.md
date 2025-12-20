# Using colors

Gama has a special `gmColor` type it uses to store colors, 

```c
typedef unsigned int gmColor
```
You can use predefined colors, see <a href="/reference/color_8h.html#header-doc-define-members">reference</a>
there are more than a hundred of colors there like `GM_RED`, `GM_BURLYWOOD`, and so on.

```c
gmColor blue = GM_BLUE;

gmColor red = 0xFF0000FF;


gmColor customColor = gm_rgb(123, 255, 20);

```

## Editing colors

You can modify colors with `gm_set_green`, `gm_set_red`, `gm_set_blue` and `gm_set_alpha`
which receive the color and number from 0 to 255.

You can do bitwise operations on color objects which result in merging colors or altering them.

```c
gmColor magenta = red | blue;
gmColor dark_magenta = magenta & GM_GRAY;
gmColor translucent_red = red & GM_TRANSLUCENT;
```

## Using color codes

To learn more about the color codes used in gama you can view the 
[w3schools color reference](https://www.w3schools.com/colors/).

Colors related functions and types, with the list of builtin colors can be found in
[color.h reference](http://localhost:3002/reference/color_8h.html)
