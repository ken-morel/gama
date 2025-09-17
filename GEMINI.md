# Project Overview

This project, named "Gama," is a command-line interface (CLI) tool written in Go. Its primary purpose is to simplify the process of building and managing C-based projects, particularly games, for both Linux and Windows platforms. It uses the `cobra` library for command-line parsing and `viper` for configuration management. A key feature of Gama is its ability to cross-compile C projects for Windows from a Linux environment using `winegcc`.

The tool provides project templates, such as a "pong" game, to help users get started quickly. It abstracts away the underlying build complexities, allowing developers to focus on writing C code. On Windows, it utilizes the WinAPI, and on Linux, it uses GLFW.

# Building and Running

The project is a Go application and can be built and run using the standard Go toolchain.

## Building the `gama` CLI

To build the `gama` CLI tool itself:

```sh
go build -o gama main.go
```

## Using the `gama` CLI

Once the `gama` CLI is built, you can use it to manage C projects.

### Creating a New Project

To create a new C project using a template:

```sh
./gama create <project_name>
```

### Building a C Project

To build the C project:

```sh
./gama build
```

To build the C project and then run it:

```sh
./gama build -r
```

### Cross-Compiling for Windows (from Linux)

To build a Windows executable from a Linux environment:

```sh
./gama build -w
```

# Development Conventions

*   **CLI:** The project uses the `cobra` library to structure its command-line interface. New commands should be added in the `cmd/` directory.
*   **Configuration:** The `viper` library is used for configuration management, allowing for settings to be defined in a `gama.yml` file.
*   **Project Structure:** The core Go logic is located in the `gama/` directory, while the CLI command definitions are in the `cmd/` directory. C project templates are stored in `assets/templates/`.
*   **Cross-Compilation:** The tool is designed to be cross-platform, with a focus on making it easy to build Windows executables on Linux.
