# Gama widgets

If you are already familiar to [drawing shapes](/docs/learn/drawing),
widgets are more complex user interface elements which compose of shapes,
gama provides a few built in widgets so you don't have to craft them yourself.

Since gama is imediate-mode, you draw shapes using simple functions, and if
you want to configure the shapes more(like colors, borders) shapes
have global structures like `gmwFrame` which you can change.

## Saving and restoring the theme

If you want to change a single object's theme, you can temporarily change the theme,
using `gmw_save()`, and then restore it with `gmw_restore()`, you can save and restore
several times(by the time of this writing, you can save 10 times),
but you can configure that by defining <a href="/reference/widgets_8h.html#a1c14c94de7b1d5b10cdab5ffd123b0f8">`gama_max_theme_CACHE`</a>

```c
gmw_save(); // save default theme #0
// configure theme for first group of widgets
gmwFrame.border = GM_RED;
// draw frames
// configure for a single frame
gmw_save(); // save #1
fmwFrame.enabled = 0;
gmw_frame(...);
gmw_restore(); // restore last save, i.e #1
// draw other frames
 
gmw_restore(); // restore last save i.e default theme #0
```

## Reference

- <a href="/reference/widgets_8h.html">widgets.h reference</a>
- <a href="/reference/dir_23e0dee3fd6f26038bcbe9982731bc10.html">widgets/ references</a>
