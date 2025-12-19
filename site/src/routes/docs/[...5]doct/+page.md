# Gama Engine Documentation

## Table of Contents
1. [Introduction](#introduction)
2. [Getting Started](#getting-started)
3. [Core Concepts](#core-concepts)
4. [API Reference](#api-reference)
5. [Advanced Features](#advanced-features)
6. [Best Practices](#best-practices)

## Introduction

The Gama Engine is a lightweight, minimalist game engine designed for simplicity and speed. It combines a powerful C library for game logic with a modern toolchain written in V, providing a fast, fun, and productive development experience. With an embedded TCC compiler, Gama is a zero-dependency tool that allows you to create, build, and run your games right out of the box.

### Key Features
- **Simple C API**: A straightforward, easy-to-learn C library for creating your game
- **2D Physics Engine**: Built-in support for bodies, shapes, and collision detection
- **Rendering Primitives**: Functions for drawing shapes, lines, images, and text
- **Animation Helpers**: A set of functions to easily animate values over time
- **Modern CLI Tool**: A simple and fast project manager written in V
- **Zero-Dependency**: Comes with a built-in TCC compiler for a hassle-free setup
- **Cross-Platform (in development)**: Aims to support Windows, Linux, and Web

## Getting Started

### Prerequisites
- V language compiler installed

### Creating Your First Project
1. **Create a new project:**
   ```bash
   gama create
   ```
   This will launch an interactive assistant to help you name your project and choose a template.

2. **Run the development server:**
   ```bash
   cd your_project_name
   gama dev
   ```
   This command will build and run your project. It also watches for any changes in your `src/` directory and automatically rebuilds and restarts the application, giving you a live-reloading development environment.

### Basic Project Structure
```
project_name/
├── gama.toml          # Project configuration file
├── include/           # Gama library headers
├── src/               # Source code (mainly C files)
│   └── main.c         # Entry point
├── assets/            # Game assets
└── build/             # Compiled executables
```

### Basic Game Loop Template
```c
#include <gama.h>

int main() {
  // Initialize the engine with window dimensions and title
  gm_init(500, 500, "My Gama Game");
  
  // Game loop
  do {
    // Your game logic and rendering code here
  } while (gm_yield());
  
  return 0;
}
```

## Core Concepts

### 1. Initialization and Game Loop
- `gm_init(width, height, title)`: Initializes the engine and creates a window
- `gm_yield()`: Processes events, updates input state, and prepares for the next frame. Returns 1 if game should continue, 0 if window closed
- `gm_quit()`: Closes the window and terminates the engine

### 2. Physics System
Gama provides a comprehensive 2D physics system with:
- **Bodies**: Objects that can have mass, velocity, and acceleration
- **Collision Detection**: Automatic detection of collisions between bodies
- **Collision Resolution**: Physics-based response to collisions with restitution and friction

### 3. Rendering System
The engine provides immediate-mode rendering for various primitives:
- Basic shapes: lines, rectangles, circles, ellipses, triangles
- Rounded rectangles
- Images and text
- Body-based drawing functions

### 4. Animation System
Built-in animation functions that provide different easing options:
- Spring-like motion (exponential ease-out)
- Linear interpolation with constant speed
- Ease-in and ease-out quadratic and cubic functions
- Sine and cosine wave animations

### 5. Beyond Games: Data Visualization
Gama is not just for games! As demonstrated by the lineup project, it's excellent for creating interactive data visualization tools. The same rendering, animation, and input systems can be used for:
- Scientific simulations
- Data visualization
- Interactive educational tools
- Mathematical concept demonstrations
- Graphing applications

The grid drawing example from lineup shows how to create coordinate systems for plotting data.

## API Reference

### Basic Engine Functions

#### `gm_init(int width, int height, const char *title)`
Initializes the Gama engine and opens a window with the specified dimensions and title.
```c
gm_init(800, 600, "My Game");
```

#### `gm_yield()`
Processes events, updates input state, and prepares for the next frame.
- Returns 1 if the game should continue to the next frame
- Returns 0 if the window has been closed
```c
while (gm_yield()) {
  // Game logic here
}
```

#### `gm_quit()`
Closes the window and terminates the Gama engine.

#### `gm_background(gmColor c)`
Sets the background color of the window.

#### `gm_show_fps(int show)`
Shows or hides FPS counter in the bottom-left corner of the window.

#### `gm_fullscreen(int fullscreen)`
Toggles fullscreen mode (1 for fullscreen, 0 for windowed).

#### `gm_log(const char *txt)`
Outputs a message to the log.

### Drawing Functions

#### Basic Primitives
- `gm_draw_line(double x1, double y1, double x2, double y2, double thickness, gmColor c)` - Draws a line segment
- `gm_draw_rectangle(double x, double y, double w, double h, gmColor c)` - Draws a rectangle
- `gm_draw_rounded_rectangle(double x, double y, double w, double h, double r, gmColor c)` - Draws a rectangle with rounded corners
- `gm_draw_circle(double center_x, double center_y, double radius, gmColor c)` - Draws a circle
- `gm_draw_ellipse(double x, double y, double w, double h, gmColor c)` - Draws an ellipse
- `gm_draw_triangle(double x1, double y1, double x2, double y2, double x3, double y3, gmColor c)` - Draws a triangle
- `gm_draw_image(gmImage img, double x, double y, double w, double h)` - Draws an image
- `gm_draw_text(double x, double y, const char *text, const char *font, double font_size, gmColor c)` - Draws text

#### Body-Based Drawing Functions
These functions draw shapes using the properties of physics bodies:
- `gm_draw_body(const gmBody *body, gmColor c)` - Draws a physics body based on its collider type
- `gm_draw_rect_body(const gmBody *body, gmColor c)` - Draws a rectangular physics body
- `gm_draw_circle_body(const gmBody *body, gmColor c)` - Draws a circular physics body
- `gm_draw_image_body(const gmBody *body, gmImage img)` - Draws an image at a physics body's position

### Physics System

#### Body Creation
- `gm_rectangle_body(double m, double x, double y, double w, double h)` - Creates a rectangular physics body
- `gm_circle_body(double m, double x, double y, double r)` - Creates a circular physics body
- `gm_body_create(double mass, double x, double y, double w, double h, gmColliderType c)` - Creates a general physics body

#### Body Properties
- `gm_max_speed(gmBody *body, double max_speed)` - Sets maximum speed limit for a body
- `gm_min_speed(gmBody *body, double min_speed)` - Sets minimum speed limit for a body
- `gm_speed(gmBody *body, double speed)` - Sets the speed of a body

#### Body Boundary Functions
- `gm_body_bound_clip(gmBody *body, double bx, double ex, double by, double ey)` - Clips body to a bounding box
- `gm_body_bound_reflect(gmBody *body, double bx, double ex, double by, double ey)` - Makes body reenter on the other side when exiting bounds
- `gm_body_bound_bounce(gmBody *body, double bx, double ex, double by, double ey, double restitution)` - Makes body bounce off boundaries

### Animation System

#### Animation Functions
- `gm_anim_spring(double *value, double target, double dt, double t)` - Spring-like motion (exponential ease-out)
- `gm_anim_linear(double *value, double target, const double dt, const double t)` - Linear interpolation with constant speed
- `gm_anim_ease_out_quad(double *value, const double target, const double dt, double t)` - Quadratic ease-out
- `gm_anim_ease_out_cubic(double *value, double target, double dt, double t)` - Cubic ease-out
- `gm_anim_ease_in_quad(double *value, double target, double dt, double t)` - Quadratic ease-in

#### Wave Animation Functions
- `gm_anim_sin(double center, double radius, double speed, double offset)` - Sine wave animation
- `gm_anim_cos(double center, double radius, double speed, double offset)` - Cosine wave animation

### Input System

#### Key Input
- `gm_key(char key)` - Checks if a key is pressed (using shortcut notation)
- `gm_key_pressed(char t, char k)` - Checks if a specific key is pressed

#### Key Shortcuts
Gama uses the following key shortcuts:
- `U`, `D`, `L`, `R` for arrow keys (Up, Down, Left, Right)
- `E` for Spacebar
- `S`, `C`, `A` for mouse buttons (S=Left, C=Middle, A=Right)
- Any other character represents the corresponding key

#### Mouse Input
Mouse input can be accessed through global variables:
- `gm_mouse.position.x`, `gm_mouse.position.y` - Current mouse position
- `gm_mouse.movement.x`, `gm_mouse.movement.y` - Mouse movement since last frame
- `gm_mouse.down` - Whether mouse button is held down
- `gm_mouse.pressed` - Whether mouse button was pressed this frame

### Color System

#### Color Creation
Colors can be created using predefined constants:
- `GM_WHITE`, `GM_BLACK`, `GM_RED`, `GM_GREEN`, `GM_BLUE`
- `GM_YELLOW`, `GM_CYAN`, `GM_MAGENTA`, `GM_ORANGE`
- `GM_PINK`, `GM_PURPLE`, `GM_BROWN`, `GM_GRAY`
- `GM_LIGHT_GRAY`, `GM_DARK_GRAY`

#### Custom Colors
Colors can be created using `gm_rgba(r, g, b, a)` function with values from 0-255.

### Systems and Timing

#### Time Functions
- `gm_dt()` - Returns the time elapsed since the last frame (delta time)
- `gm_t()` - Returns the total elapsed time since the start of the program

#### Systems
Physics systems manage groups of bodies and their interactions:
- Systems can have global acceleration, damping, and velocity
- Bodies in a system are updated together
- Collision detection and resolution happen within the system context

## Advanced Features

### Physics Body Management
Physics bodies in Gama can be managed with various properties:

```c
// Create a dynamic rectangular body
gmBody player = gm_rectangle_body(1.0, 100, 100, 50, 50);  // mass=1.0, x=100, y=100, width=50, height=50

// Modify body properties directly
player.velocity.x = 5.0;  // Set horizontal velocity
player.restitution = 0.8; // Set bounciness
player.friction = 0.1;    // Set friction coefficient

// Make a body static (won't move from physics)
player.is_static = 1;

// Deactivate a body (won't participate in physics)
player.is_active = 0;
```

### Data Visualization Examples
Gama is excellent for data visualization. Here's how to draw a coordinate grid system, similar to the lineup project:

```c
void draw_coordinate_grid() {
  gmColor transparent_white = gm_set_alpha(GM_WHITE, 100);
  gmColor more_transparent_white = gm_set_alpha(GM_WHITESMOKE, 20);

  // Draw x and y axes
  gm_draw_line(-5, 0, 5, 0, 0.005, transparent_white);  // x-axis
  gm_draw_line(0, -5, 0, 5, 0.005, transparent_white);  // y-axis

  // Draw grid lines and labels for x-axis
  for (double x = -5; x <= 5; x += 0.1) {
    gm_draw_line(x, -5, x, 5, 0.01, more_transparent_white);
    if ((int)(x * 10) % 5 == 0) {  // Draw labels at every 0.5 unit
      char label[10];
      sprintf(label, "%.1lf", x);
      gm_draw_line(x, 0, x, -0.04, 0.01, transparent_white);
      gm_draw_text(x, -0.09, label, "", 0.05, transparent_white);
    }
  }

  // Draw grid lines and labels for y-axis
  for (double y = -5; y <= 5; y += 0.1) {
    gm_draw_line(-5, y, 5, y, 0.01, more_transparent_white);
    if ((int)(y * 10) % 5 == 0) {  // Draw labels at every 0.5 unit
      char label[10];
      sprintf(label, "%.1lf", y);
      gm_draw_line(0, y, -0.04, y, 0.01, transparent_white);
      gm_draw_text(-0.09, y, label, "", 0.05, transparent_white);
    }
  }
}
```

### Interactive Point Management
Managing user-placed points for visualization:

```c
#define MAX_POINTS 50

gmPos points[MAX_POINTS];
size_t num_points = 0;
int selected_point = -1;
const double point_radius = 0.02;  // Small circle radius for points

void draw_points() {
  for (size_t i = 0; i < num_points; i++) {
    // Add slight animation to points using sine wave
    double animated_radius = gm_anim_sin(point_radius, 0.001, 1, (double)i / 5);
    gmColor color = selected_point == i ? GM_ORANGE : GM_REBECCAPURPLE;
    gm_draw_circle(points[i].x, points[i].y, animated_radius,
                   gm_set_alpha(color, 200));
  }
}

void add_point(double x, double y) {
  if (num_points < MAX_POINTS) {
    points[num_points].x = x;
    points[num_points].y = y;
    num_points++;
  }
}
```

### Animation Examples
```c
double x_pos = 0;
double target_x = 100;

// Spring animation
gm_anim_spring(&x_pos, target_x, gm_dt(), 0.5);  // 0.5 second animation

// Linear interpolation
gm_anim_linear(&x_pos, target_x, gm_dt(), 2.0);  // 2 second animation

// Sine wave animation
x_pos = gm_anim_sin(100, 50, 1.0, 0);  // Center=100, amplitude=50, speed=1.0, offset=0
```

### Input Handling Examples
```c
// Check if arrow keys are pressed
if (gm_key('U')) {  // Up arrow
    player.velocity.y = 5.0;
}
if (gm_key('D')) {  // Down arrow
    player.velocity.y = -5.0;
}
if (gm_key('L')) {  // Left arrow
    player.velocity.x = -5.0;
}
if (gm_key('R')) {  // Right arrow
    player.velocity.x = 5.0;
}

// Check for mouse click on a body
if (gm_clicked(&player)) {
    // Player body was clicked
}
```

### Collision Detection
```c
// Bodies are automatically checked for collisions in physics systems
// You can also manually check if bodies are overlapping
if (gm_body_contains(&player, x, y)) {
    // Check if point (x,y) is inside the player body
}
```

## Best Practices

### 1. Project Structure
Organize your projects following the pattern shown in the lineup example:
```
project_name/
├── gama.toml          # Project configuration
├── include/           # External headers (if needed)
├── src/               # Source code files
│   ├── main.c         # Main entry point
│   ├── components.h   # Custom component headers
│   └── utils.h        # Utility functions
├── assets/            # Game assets
└── build/             # Compiled executables
```

### 2. Game Loop Structure
Always follow the recommended game loop pattern:
```c
int main() {
  gm_init(800, 600, "My Game");

  // Initialize game objects
  gmBody player = gm_rectangle_body(1.0, 400, 300, 32, 32);

  do {
    // Update game logic
    // Handle input
    // Update physics
    // Draw everything

  } while (gm_yield());

  return 0;
}
```

### 2.1 Real-World Example: Data Visualization Loop
As shown in the lineup project, for data visualization applications:
```c
int main() {
  gm_init(700, 500, "Lineup");
  gm_background(GM_BLACK);
  gm_fullscreen(1);
  gm_show_fps(1);

  // Initialize data points and algorithm variables
  // Gradient descent parameters
  double gradient = 0, intercept = 0;

  do {
    // Handle user input for data points
    // Update regression algorithm
    // Visualize the results

  } while (gm_yield());

  return 0;
}
```

### 3. Delta Time Usage
Always use `gm_dt()` for time-based calculations to ensure consistent game speed across different frame rates:
```c
// Move object at 100 pixels per second
player.position.x += 100 * gm_dt();
```

### 4. Physics Body Management
- Set static bodies for objects that shouldn't move (platforms, walls)
- Use appropriate masses for different objects (heavier objects should have higher mass)
- Consider using boundary functions to keep objects within game bounds

### 4. Animation Best Practices
- Use spring animations for responsive UI elements
- Use linear animations for precise timing requirements
- Use wave animations for cyclic movements (pendulums, oscillators)

### 5. Memory Management
- Physics systems and bodies are managed automatically
- Be careful with dynamic allocation if you extend beyond the basic API
- Clean up any custom resources before the program exits

## CLI Commands Reference

| Command | Description |
|---------|-------------|
| `gama create` | Starts an interactive assistant to create a new project |
| `gama build` | Compiles your project into an executable in the `build/` dir |
| `gama run` | Runs a previously built project |
| `gama dev` | Builds and runs the project, with auto-rebuild on changes |
| `gama lib reset` | Reset the project's gama library to the CLI tool's version |
| `gama package` | Package the current gama project into a setup |

## Project Configuration

The `gama.toml` file contains project-specific configurations:

```toml
name = "project_name"
description = "A sample gama project"
version = "0.0.0"
author = "author_name"

[gama]
version = "0.1.0"
compiler = "path_to_compiler"  # Leave empty to use built-in TCC
```

## Template Projects

Gama provides several templates to get started quickly:
- **skeleton**: A minimal project template
- **pong**: A simple Pong game example
- **pong_advance**: An advanced Pong game with more features

## Applications Beyond Games

As demonstrated by the lineup project, Gama is highly suitable for non-game applications:

### Educational Tools
- Interactive mathematics visualization
- Physics simulations
- Scientific data visualization
- Algorithm demonstrations

### Scientific Applications
- Plotting and graphing tools
- Simulation environments
- Data analysis interfaces
- Research visualization tools

## Contributing This Documentation

This comprehensive documentation was created to enhance the Gama Engine project. It can be contributed to the main repository to improve the project's accessibility and adoption. The documentation covers all major features and APIs, providing both beginners and advanced users with the information they need to use the Gama Engine effectively.

To contribute this documentation to the main project:
1. Fork the original repository
2. Add this documentation file (gama_docs.md)
3. Update the main README.md to reference this documentation
4. Submit a pull request with these changes

This would significantly improve the project's documentation situation compared to the current state where only a minimal README.md exists.

### Key Advantages for Non-Game Applications
- **Immediate Mode Rendering**: Efficient for dynamic visualizations
- **Built-in Animation System**: Perfect for showing evolving processes
- **Simple Input Handling**: Easy to implement user interaction
- **Cross-Platform**: Deploy to multiple platforms with minimal changes
- **Zero Dependencies**: Self-contained deployment with built-in compiler

## Real-World Example: Lineup - Linear Regression Tool

The "lineup" project is an excellent example of a real-world application built with Gama. It's a graphical, animated linear regression tool that demonstrates how to use Gama for data visualization rather than just games.

### Key Features of Lineup:
- **Interactive Data Points**: Users can click and drag to add and move data points
- **Real-time Linear Regression**: The line automatically adjusts to fit the user's data points
- **Learning Algorithm Visualization**: Shows how gradient descent works visually
- **Grid System**: Includes coordinate grid with labeled axes for precise positioning
- **Animated Elements**: Uses Gama's animation system for smooth visual feedback

### Implementation Details:
- Located in `Projects/lineup/` directory
- Uses custom headers for different components:
  - `gridlines.h`: Draws coordinate grid system
  - `line.h`: Implements linear regression algorithm and plotting
  - `user_points.h`: Manages user-placed data points
  - `utils.h`: Contains utility functions for display

### Linear Regression Algorithm:
The project implements gradient descent for linear regression with:
- Dynamic calculation of gradient and intercept: `y = gradient * x + intercept`
- Real-time loss calculation (mean absolute error)
- Visual feedback showing current loss value
- Animated line that adjusts to best fit the user data points

### User Interface Elements:
- Fullscreen mode with black background for good contrast
- Interactive grid with labeled axes
- Color-coded data points (orange for selected, purple for others)
- Real-time display of the regression equation and loss value
- Animate points with sine wave for visual appeal

## Future Enhancements

Based on the project roadmap, future enhancements include:
- An `int gm_draw_cache(char *name)` function for caching static drawings
- Cross-platform support for Windows, Linux, and Web
- Additional rendering and physics features

## Troubleshooting

### Common Issues:
1. **Build fails**: Check that your gama.toml file is properly configured
2. **Performance issues**: Use appropriate delta time scaling and limit physics updates
3. **Physics issues**: Make sure bodies have appropriate mass values and boundary conditions
4. **Input not responding**: Ensure input handling is done in the correct part of the game loop

### Debugging Tips:
- Use `gm_show_fps(1)` to monitor performance
- Add logging with `gm_log()` to track game state
- Use boundary functions to keep objects visible on screen
- Test with simple physics bodies before implementing complex behaviors

### Getting Help
- This documentation file: For comprehensive coverage of Gama Engine features
- Main README.md: For quick getting started instructions
- Issues on GitHub: To report bugs or request features
- Pull requests: To contribute improvements like this documentation
