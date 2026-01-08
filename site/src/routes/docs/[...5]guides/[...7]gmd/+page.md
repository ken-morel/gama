# Introspection and Debugging


Gama includes a set of functions to help you debug your code.
They include `gmd_` functions, like `gmd_pos3`, `gmd_pos2`, ... which writes
a string representation of the object.

```c
#include <gama.h>

gmPos pos = {1.0};

char text[200] = {0};
gmd_pos2(text, sizeof(text), pos);
```

Gama also includes helper macros which print directly the objects.
they include:

- `gmd`: Takes the second part of the function name(e.g `pos2` instead of `gmd_pos2`), the object
  you want to print and an ending character, like a newline to print right after the object.
- `gmdn`: This calls gmd, with a newline as third argument, i.e it prints the object with a '\n'
  at the end.
- `gmdi`: prints the object without a newline at the end.

```c
gmPos p = {0};

gmd(pos2, p, "end");
// <gmPos(0, 0, 0)>end

gmdn(pos2, p);
// <gmPos(0, 0, 0)>
//                // <- a newline here


gmdi(pos2, p);
// <gmPos(0, 0, 0)>
```

Several other printing functions exist, and you can have them in the [debug.h reference](/reference/debug_8h.html).
