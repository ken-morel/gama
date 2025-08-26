# Installation

To use gama, the easiest way is to install the gama command line tool to easily get started
building projects.

## Linux Installation

Linux debian binaries are available at the [gama releases page](https://github.com/ken-morel/gama/releases).
Download the archive then install with apt.

```bash
sudo apt install path/to/debian/archive.deb
```

This will install gama and required development libraries like glfw, then you
can use gama via `gama` command on your terminal.

```fish
─>$ gama
2025/08/26 01:31:15 open gama.yml: no such file or directory
Gama wraps along the whole project of creating, running and packaging
 your apps using gama.h.

Usage:
  gama [command]

Available Commands:
  build       Build the applicaiton
  completion  Generate the autocompletion script for the specified shell
  create      Create a new gama project
  font        Manage project fonts
  help        Help about any command
  package     A brief description of your command
  run         Run the built binaries

Flags:
      --config string   config file (default is $HOME/.gama.yaml)
  -h, --help            help for gama
  -t, --toggle          Help message for toggle

Use "gama [command] --help" for more information about a command.
```

## Windows installation

You can install on windows using the windows installer at the [releases page](https://github.com/ken-morel/gama/releases). Follow the setup to install gama user wide.
