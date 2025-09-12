
# Gama

[![Deploy mdBook site to Pages](https://github.com/ken-morel/gama/actions/workflows/mdbook.yml/badge.svg)](https://github.com/ken-morel/gama/actions/workflows/mdbook.yml)

Gama is a beginner-friendly toolkit and command-line utility for building native C projects—especially games—on both Linux and Windows. It provides a streamlined workflow, project templates (like Pong), and cross-platform build capabilities. Gama is aimed at high-schoolers and anyone with C basics who wants a fast, modern project setup.

---

## Features

- **Cross-platform:** Works on Linux and Windows.
- **Seamless Windows builds on Linux:** Uses `winegcc` for compiling Windows binaries from Linux.
- **Backend flexibility:** Uses WinAPI on Windows and GLFW on Linux.
- **Project templates:** Quickly start projects like Pong, skeleton apps, and more.
- **Easy CLI:** `gama` command to create, build, and manage projects.
- **Beginner-friendly:** Simple structure and extensive documentation.

---

## Quick Start

### Installation

See the full [installation guide](https://gama.readthedocs.io/en/latest/installation.html) for details.

#### On Linux

```sh
# Clone the repo and build
git clone https://github.com/ken-morel/gama.git
cd gama
make
sudo make install
```

#### On Windows

Download the latest release from [Releases](https://github.com/ken-morel/gama/releases) and follow the [Windows instructions](https://gama.readthedocs.io/en/latest/installation.html#windows).

---

## Creating a New Project

Use the `gama` command-line utility to quickly bootstrap a new project:

```sh
gama new pong MyPongGame
cd MyPongGame
make
./MyPongGame
```

Available templates include:
- **pong** — A complete Pong game in C
- **skeleton** — Minimal starter project

See the [Templates Guide](https://gama.readthedocs.io/en/latest/gamatool.html#templates) for the full list.

---

## Building for Windows on Linux

Gama supports cross-compiling Windows binaries directly from Linux using `winegcc`:

```sh
gama build --platform=windows
```

This produces a Windows executable using the WinAPI backend, with no need for a Windows machine.

---

## What Does Gama Code Look Like?

Here’s a snippet from the Pong template:

```c
#include "gama.h"

void update(GameState *state) {
    // Game logic here
}

int main() {
    GamaConfig config = gama_default_config();
    gama_run(&config, update);
    return 0;
}
```

Gama abstracts away platform details, so you focus on writing portable C code. On Windows, Gama uses WinAPI; on Linux, it uses GLFW automatically.

---

## Command-Line Usage

See all available commands with:

```sh
gama help
```

**Common commands:**
- `gama new <template> <project_name>` — Create a new project
- `gama build` — Build the current project
- `gama run` — Build and run
- `gama build --platform=windows` — Cross-compile for Windows (from Linux)

See the [Gama CLI Reference](https://gama.readthedocs.io/en/latest/gamatool.html) for more.

---

## Documentation

- **Full docs:** [gama.readthedocs.io](https://gama.readthedocs.io)
- **Getting started:** [Welcome](https://gama.readthedocs.io/en/latest/welcome.html)
- **Installation:** [Installation Guide](https://gama.readthedocs.io/en/latest/installation.html)
- **Templates:** [Project Templates](https://gama.readthedocs.io/en/latest/gamatool.html#templates)
- **API Reference:** [Reference](https://gama.readthedocs.io/en/latest/reference.html)

---

## Example Projects

- [Pong Game Template](assets/templates/pong/README.md)
- [Skeleton Template](assets/templates/skeleton/README.md)

---

## Contributing

Contributions are welcome! Check [CONTRIBUTING.md](CONTRIBUTING.md) or open an issue to get started.

---

Gama is designed to help new programmers build and run real native C projects on any desktop platform, with minimal setup and maximum fun. Happy coding!
```


