# Gama Engine Project Context

## Project Overview

Gama is a lightweight, minimalist game engine designed for simplicity and speed. It integrates a C library for core game logic with a modern command-line interface (CLI) toolchain written in V language. A key feature is its embedded TCC (Tiny C Compiler), which makes it a zero-dependency tool for creating, building, and running games.

**Key Technologies:**
*   **C:** Used for implementing game logic via a simple API.
*   **V:** Powers the modern and fast CLI project manager.
*   **TCC:** An embedded C compiler providing a hassle-free, zero-dependency setup.

**Architecture:**
The project is structured around a core C library (`lib/gama.h`), a V-lang based CLI tool (defined in `cli.v` and `vgama/gama.v`), various runners (native, TCC, and web in `runners/`), and project templates (e.g., `templates/pong`, `templates/skeleton`) that demonstrate how to use the engine.

## Building and Running

The `gama` CLI tool is the primary interface for managing projects. Below are the key commands:

*   **`gama create`**: Initiates an interactive assistant to set up a new project, allowing you to name it and select a template.
*   **`gama dev`**: Builds and runs the project in development mode. It watches for changes in the `src/` directory, automatically rebuilds, and restarts the application for a live-reloading experience.
*   **`gama build`**: Compiles your project into an executable, typically located in a `build/` directory within your project.
*   **`gama run`**: Executes a project that has been previously built.

## Development Conventions

*   **Game Logic:** Game-specific logic is primarily written in C, typically within `src/` directories of individual projects created from templates.
*   **Tooling:** The project's internal tooling and CLI are developed using the V programming language.
*   **Contribution:** Contributions are welcomed, following standard open-source practices (issues, pull requests).
