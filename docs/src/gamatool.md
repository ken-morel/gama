# The gama tool

The gama tool manages and creates gama projects.

## Creating a project

To create projects, `gama create` is the key. It takes as arguments the project name, and optionally the template you want to use.

```fish
─>$ gama create --help
Create is a command to create or start a new utility.

 Use:
 gama create <name>

 It creates a new directory called <name> and initializes the
 project in it.

Usage:
  gama create <name> [flags]

Aliases:
  create, init, initialize

Examples:
gama create helloWorld

Flags:
  -h, --help              help for create
  -n, --name string       The name of the project you want to create
  -t, --template string   The starter template (default "skeleton")

Global Flags:
      --config string   config file (default is $HOME/.gama.yaml)
```

To create a project called `myApp` with the default template:

```bash
gama create myApp
```

To specify a template like pong, at a `-t <templatename>` to the command:

```bash
gama create myApp -t pong
```

## Building your project

To build a project from your terminal:

- Open your terminal, like cmd, kitty, or windows terminal
- Change to your project root(The folder you created) using `cd my/project/path` on linux or powershell or `dir my/project/path` on windows.
- Run the build command `gama build`

To build the project run `gama build`, this will create a folder in `build/` and your executable application will be in that folder.

> [!NOTE]
> You may not run the executable inside the build folder, as it will have errors trying to find your app files.
> to run your project after building: run `gama run`.
> To build and run your project directly, run `gama build -r` this is what you want most of the case.

### Building for windows, ... on linux

Building for windows on linux required `winegcc` installed, on ubuntu run, install it with apt(`sudo apt install winegcc`), and to run a windows project you need `wine` command intalled, once that's done, simply add `-w` to the build or run commands to build for windows.
