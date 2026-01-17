# Gemini Development Guide for the Gama Project

This document outlines the conventions and workflows for using Google's Gemini models to assist in the development of the Gama engine. Its purpose is to ensure that AI-assisted contributions are consistent, safe, and effective.

## Introduction

The Gama project leverages Gemini for a variety of development tasks, including:
-   **Bug Fixing**: Analyzing issues and implementing fixes.
-   **Feature Implementation**: Scaffolding and building new features.
-   **Code Documentation**: Generating and updating Doxygen comments.
-   **Refactoring**: Improving code structure and clarity.
-   **Brainstorming**: Suggesting new features and architectural improvements.

This guide helps both human developers and the AI model to stay aligned with the project's standards.

## Core Principles for AI Collaboration

When using Gemini, we adhere to the following principles:

1.  **Human in the Loop**: The developer is always in control. AI-generated code should be treated as a suggestion from a pair programmer—it must be reviewed, understood, and approved before being committed.
2.  **Existing Conventions First**: Gemini is instructed to strictly follow the existing coding style, architecture, and conventions of the project. It should analyze the surrounding code before making any changes.
3.  **Iterative and Specific Prompts**: Vague requests like "fix the bug" are discouraged. Instead, developers should provide specific, iterative prompts, often including error messages, file contents, and clear objectives.
4.  **Verification is Key**: The developer is responsible for testing and verifying all changes. This includes compiling the code, running tests, and checking for regressions.

## Technical Workflow

Gemini interacts with the project via a command-line interface that provides a set of tools to read files, write files, and execute shell commands.

### Common Tasks & Commands

-   **Understanding the Code**: To understand a file, Gemini will typically use `read_file{file_path: "path/to/file.h"}`.
-   **Making Changes**: Changes are made with `replace` (for targeted edits) or `write_file` (for overwriting a file with significant changes). The `replace` tool is preferred for its precision.
-   **Build & Test**: Gemini can use the `mng` script to build the project and run tests, for example: `run_shell_command{command: "./mng build"}`.
-   **Complex Analysis**: For high-level analysis, root cause investigation, or large-scale refactoring, Gemini can use its `codebase_investigator` sub-agent.

### Project-Specific Conventions

Gemini has been made aware of the following conventions during its work on the project:

-   **Language**: The core engine is written in **C99-style C**. The build script and CLI tool are written in **Vlang**. The web frontend is written in **TypeScript**.
-   **Documentation**: All public-facing C code in header files should be documented using **Doxygen** format (`/** ... */`).
-   **Build System**: The primary build and task runner is the `./mng` script. It handles builds for different platforms, documentation generation, and packaging.
-   **3D Rendering**: The 3D renderer is a **software-based rasterizer**. It does not use GPU-specific APIs like OpenGL or Vulkan directly for rendering logic, instead outputting to a 2D triangle buffer.
-   **Web Platform**: The web build uses **WebAssembly** for the core engine and a **2D Canvas** for rendering. Communication between the WASM module and the JavaScript frontend is handled via a **Web Worker** and `postMessage`.

## Example Interaction

Here is a typical workflow for fixing a bug with Gemini:

1.  **Developer**: "Hello Gemini, I'm getting a `SIGSEGV` when loading GLTF files. The error seems to be in the loading or deserialization phase. Please investigate."
2.  **Gemini**: "Understood. The `SIGSEGV` suggests a memory corruption issue, likely in the GLTF loader. I will start by examining `lib/gama/3d/gltf.h` to find the cause."
    ```
    read_file{file_path: "lib/gama/3d/gltf.h"}
    ```
3.  **Gemini**: "I've found a potential buffer overflow in the vertex counting logic. I will rewrite the function to use a safer, dynamic array-based approach."
    ```
    write_file{file_path: "lib/gama/3d/gltf.h", content: "..."}
    ```
4.  **Developer**: Approves the `write_file` operation.
5.  **Gemini**: "The fix has been applied. Please re-compile and test to confirm the `SIGSEGV` is resolved."

This document serves as a living guide and should be updated as the project's architecture and development practices evolve.
