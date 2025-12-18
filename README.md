# Gama

![Gama logo](./gama.svg)

website: https://gama.rbs.cm

Gama project aims to provide a convenient way for students and beginers in
C, to ship portable and safe 2d games, putting accent on simplicity, safety
and control.

[Gama cli](https://gama.rbs.cm/cli) is a command line tool built in [V](https://github.com/vlang/v) which
provides a build system and project manager for gama projects.

Gama.h, a gama library providing an easy to use immediate mode interface
to render game components, gama.h is tailored to adapt to web assembly
or [`vgama` shared library](https://gama.rbs.cm/vgama).

## Principles, or design

- **stack more, heap less**: gama aims to let students rely on
  automatic stack allocation and deallocation in functions.
- **you have control**: Alot of work is put in gama backend
  so that your code owns a mainloop.
- **no global state/scenes**: Functions can be different scenes with
  their initialization, destruction and objects, getting automatically dropped.
- **imediate mode**: Reducing further the need for pointers, you don't create
  shapes to draw when needed, you draw the shapes, this may increase the work
  that you have to do, but makes it simplier and efficient for a game.

## What does it look like?

sample from [line up project](https://github.com/ken-morel/lineup)

```c
#include <gama.h>
#include <math.h>
// ...includes


int main() {
  gm_init(800, 500, "Lineup");
  gm_background(GM_BLACK);
  gm_fullscreen(1);
  gm_show_fps(1);

  double learn_scaled = sqrt(sqrt(learn_rate)),
         learn_anim = learn_scaled;

  do {
    draw_gridlines();
    show_text_messages();
    plot_user_points();
    plot_line();

    int controls_hovered = gmw_frame(1, 0.75, 0.45, 0.36);
    gmw_switch_anim(0.9, 0.85, 0.18, 0.09, &autoplay, &swanim);
    gm_draw_text(1.1, 0.85, "auto", "", 0.1, GM_WHITE);
    gmw_scale_anim(1, 0.75, 0.4, 0.02, &learn_scaled, &learn_anim);
    if (gmw_button(1, 0.65, 0.2, 0.08, "step", 0.1) && gm_mouse.down)
      one_epoch();
    learn_rate = pow(learn_scaled, 4);

    if (gm_mouse.pressed && selected_point == -1) {
      if (!controls_hovered && !joy_hovered)
        add_user_point(gm_mouse.position.x, gm_mouse.position.y);
    } else if (gm_mouse.down) {
      if (selected_point >= 0)
        user_points[selected_point] = gm_mouse.position;
    } else if (gm_key('d') || gm_key_pressed('s', 'd')) {
      delete_selected_point();
    }
    if (gm_key('f'))
      learn_scaled += 0.01;

    find_selected_point();
    if (gm_key_pressed('s', 'x'))
      gm_quit();
  } while (gm_yield());
  return 0;
}
```

## Contributing

Always nice to see pr's and issues(at least almost always); which are not mine :smiley: .


