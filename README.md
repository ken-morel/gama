# Gama

![Gama logo](./gama.svg)
[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/G2G4XYJU6)

**Website:** https://gama.rbs.cm
**Documentation:** https://gama.rbs.cm/docs

Gama project aims to provide a convenient way for students and beginners in
C, to ship portable and safe 2D games, putting emphasis on simplicity, safety
and control.

## Features

- **Lightweight C Library**: Simple, easy-to-learn C API with clear function names designed specifically for students and beginners.
- **Built-in 2D Physics Engine**: Physics system with bodies, shapes, and collision detection supporting restitution, friction, and constraints.
- **Immediate Mode Rendering**: Draw shapes directly without creating objects. Reduces pointer usage and manual memory management.
- **Comprehensive Animation System**: Multiple easing functions and wave animations for smooth, professional-looking motion.
- **Modern CLI Tool**: Fast project manager written in V with zero-dependency setup using embedded TCC compiler.
- **Advanced Input System**: Intuitive key and mouse input with predefined shortcuts for arrows, space, and mouse buttons.
- **Cross-Platform Development**: Target Windows, Linux, and Web with consistent APIs and behavior.
- **Educational Focus**: Designed specifically for teaching C programming with safety and simplicity in mind.

## Commands

This project uses the `mng` script for various tasks:

- `./mng build` - Builds the Gama CLI tool for both Linux and Windows
- `./mng docs` - Generates Doxygen documentation and integrates it into the Svelte site
- `./mng package` - Builds, compiles vgama, and creates installers/packages
- `./mng vgama` - Builds the vgama shared library for native platforms
- `./mng help` - Shows available commands

## Website & Documentation

The official website is built with SvelteKit and provides comprehensive documentation:

- **Main Site**: https://gama.rbs.cm - Features project information, design principles, and getting started guides
- **API Reference**: Automatically generated from source code using Doxygen and integrated into the site
- **Tutorials**: Step-by-step guides for installation, development, and advanced features
- **Widget Documentation**: Detailed guides for all UI components (frames, buttons, scales, joysticks, switches)

## Principles, or design

- **stack more, heap less**: gama aims to let students rely on
  automatic stack allocation and deallocation in functions.
- **you have control**: A lot of work is put in gama backend
  so that your code owns a mainloop.
- **no global state/scenes**: Functions can be different scenes with
  their initialization, destruction and objects, getting automatically dropped.
- **immediate mode**: Reducing further the need for pointers, you don't create
  shapes to draw when needed, you draw the shapes, this may increase the work
  that you have to do, but makes it simpler and efficient for a game.

## What does it look like?

Sample from [line up project](https://github.com/ken-morel/lineup)

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

Always nice to see PRs and issues (at least almost always); which are not mine :smiley:.

To contribute to the documentation or the site:
- Website source code is located in the `site/` directory
- Documentation is generated automatically with Doxygen and the `mng docs` command
- Add new tutorials/pages in `site/src/routes/docs/`
- The build system automatically serves Doxygen documentation at `/reference/`
