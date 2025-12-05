# Gama Engine

![Gama Logo](./gama.png)

Gama is a lightweight, minimalist game engine designed for simplicity and speed. It combines a powerful C library for game logic with a modern toolchain written in V, giving you a fast, fun, and productive development experience.

With an embedded TCC compiler, Gama is a zero-dependency tool. You can create, build, and run your games right out of the box without installing any external compilers.

## Features

-   **Simple C API:** A straightforward, easy-to-learn C library for creating your game.
-   **2D Physics Engine:** Built-in support for bodies, shapes, and collision detection.
-   **Rendering Primitives:** Functions for drawing shapes, lines, images, and text.
-   **Animation Helpers:** A set of functions to easily animate values over time.
-   **Modern CLI Tool:** A simple and fast project manager written in V.
-   **Zero-Dependency:** Comes with a built-in TCC compiler for a hassle-free setup.
-   **Cross-Platform (in development):** Aims to support Windows, Linux, and Web.

## Getting Started

Getting started with Gama is easy. Once you have the `gama` executable, you can create your first project.

## Your First Project

1.  **Create a new project:**
    ```bash
    gama create
    ```
    This will launch an interactive assistant to help you name your project and choose a template.

2.  **Run the development server:**
    ```bash
    cd your_project_name
    gama dev
    ```
    This command will build and run your project. It also watches for any changes in your `src/` directory and automatically rebuilds and restarts the application, giving you a live-reloading development environment.

## CLI Commands

The `gama` CLI is your main tool for managing projects.

| Command       | Description                                                 |
|---------------|-------------------------------------------------------------|
| `gama create` | Starts an interactive assistant to create a new project.    |
| `gama build`  | Compiles your project into an executable in the `build/` dir. |
| `gama run`    | Runs a previously built project.                            |
| `gama dev`    | Builds and runs the project, with auto-rebuild on changes.  |

## Contributing

Gama is an open-source project and contributions are welcome! Feel free to open an issue or submit a pull request.

## License

Gama is licensed under the [MIT License](./LICENSE).
