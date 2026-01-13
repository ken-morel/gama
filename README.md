# Gama: A Simple C Game Engine

[![License](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)

Gama is a lightweight, cross-platform game engine written in C, designed with simplicity and education in mind. It aims to provide a convenient and powerful way for students and beginners to create 2D and 3D games while learning the fundamentals of C programming. The engine prioritizes a minimal, clean C API, control over the game loop, and a stack-first memory philosophy.

![Gama logo](./gama.svg)

## Philosophy

Gama is built on a few core principles to make game development in C more approachable and fun:

-   **Simplicity Over Features**: The API is designed to be small, intuitive, and easy to learn, avoiding complex abstractions and boilerplate.
-   **You Have Control**: Gama gives you, the developer, full control over the main game loop. The engine provides utilities but stays out of your way.
-   **Stack First, Heap Less**: The API encourages using stack-allocated data structures to leverage C's automatic memory management and reduce the cognitive load of `malloc` and `free`.
-   **No Global State**: Gama is designed to avoid global state. You can structure your game into different scenes (as simple C functions), each with its own initialization, objects, and cleanup.
-   **Immediate Mode**: The 2D rendering API follows an immediate-mode paradigm. You draw shapes directly each frame, which simplifies state management and is perfect for learning.

## Features

### Core Engine
- **Cross-Platform**: Build and run your games on **Linux**, **Windows**, and the **Web** (via WebAssembly).
- **Simple C API**: A clean, beginner-friendly C99 API.
- **Full Game Loop Control**: You own the `main` loop.
- **Custom Memory Allocator**: A static-pool-based `malloc` implementation, ideal for controlled memory environments like WASM.

### Rendering
- **2D Immediate-Mode Graphics**: Simple functions to draw primitive shapes like rectangles, circles, lines, and triangles.
- **2D Sprite Animation**: A complete sprite system with support for sprite sheets and custom animation sequences.
- **3D Software Rasterizer**: A built-in 3D rendering pipeline that projects 3D models onto a 2D image buffer, complete with lighting and backface culling.
- **3D Model Loading**: Support for loading `.obj` and `.gltf` 3D models.
- **Material & Texture Support**: The 3D pipeline supports basic material properties (diffuse color, shininess) and will use the average color of a texture if one is provided.

### Physics
- **Built-in 2D Physics Engine**: A simple physics simulation for your 2D games.
- **Collision Detection**: Supports Circle-vs-Circle, Rectangle-vs-Rectangle, and Circle-vs-Rectangle collision detection.
- **Physics Bodies**: Create bodies with properties like mass, velocity, acceleration, and restitution (bounciness).
- **Physics System**: Manage groups of bodies, apply global forces like gravity, and handle collisions automatically.

### UI & Widgets
- **Immediate-Mode UI**: A suite of skinnable, immediate-mode UI widgets.
- **Available Widgets**:
    - `gmw_button`: An interactive button.
    - `gmw_switch`: A toggle switch.
    - `gmw_scale`: A slider for selecting a value in a range.
    - `gmw_joystick`: A virtual joystick for on-screen controls.
    - `gmw_frame`: A panel for grouping UI elements.
- **Theming System**: A simple system for saving and restoring widget themes to easily change UI styles.

### And More...
- **Animation Utilities**: A collection of easing functions (`spring`, `ease-in`, `ease-out`) for smooth animations.
- **Input Handling**: Straightforward functions for checking keyboard and mouse state.
- **Math & Debug Libraries**: A custom math library and simple print-based debugging macros.

## Getting Started

### Prerequisites
- **V Compiler**: The `mng` build script is written in V. You will need to install the V compiler from [vlang.io](https://vlang.io).
- **C Compiler**: A C compiler like `gcc`, `clang`, or `tcc`.
- **Doxygen (Optional)**: For generating documentation.
- **`makensis` (Optional)**: For building the Windows installer.
- **`nfpm` (Optional)**: For creating `.deb` packages.

### Building and Running

The project uses a custom `mng` script (written in V) as a task runner for all common operations.

1.  **Build the `gama` CLI tool:**
    ```bash
    ./mng build
    ```
    This compiles the main `gama` command-line tool to the `bin/` directory.

2.  **Build the Web Version:**
    To prepare the web runner, you need to build the `gama.js` library. This requires `bun`.
    ```bash
    ./mng web
    ```
    This command builds the JavaScript components and places the necessary files in `runners/web/`.

3.  **Generate Documentation:**
    To generate the Doxygen API documentation and integrate it into the website:
    ```bash
    ./mng docs
    ```
    The generated HTML documentation will be available in `site/static/reference/`.

## Development with Gama

Here is a simple example of a "Hello, World" application in Gama to demonstrate the basic structure.

```c
#include <gama.h>

int main() {
  // Initialize the engine and create a 600x400 window
  gm_init(600, 400, "Hello Gama!");

  // Set a background color
  gm_background(GM_DARKSLATEGRAY);

  // Main game loop
  while (gm_yield()) {
    // gm_yield() handles events, clears the screen, and returns
    // true as long as the window is open.

    // Draw a red rectangle in the center of the screen
    gm_draw_rectangle(0, 0, 0.5, 0.3, GM_RED);

    // Draw some text
    gm_draw_text(0, 0, "Hello, World!", "default-ui", 0.1, GM_WHITE);
  }

  // The engine will automatically handle cleanup on exit.
  return 0;
}
```

## Project Structure
- `lib/gama/`: Contains all the core C header files for the engine's modules (3D, physics, widgets, etc.).
- `gama/`: Vlang source code for the main `gama` CLI tool and build logic.
- `gama.js/`: TypeScript source for the WebAssembly frontend and JS/C interop.
- `mng`: The main V script used for building, packaging, and managing the project.
- `runners/`: Contains the platform-specific code for running Gama applications (e.g., on web or native).
- `site/`: SvelteKit source code for the official website and documentation portal.
- `test/`: Contains example projects and test cases for the engine.

## Contributing
Contributions are welcome! Please feel free to submit a pull request or open an issue.

To contribute to the website or documentation:
- The website source code is in the `site/` directory.
- The API reference is generated automatically with Doxygen via the `./mng docs` command.

## License
This project is licensed under the MIT License - see the [LICENSE](./LICENSE) file for details.
