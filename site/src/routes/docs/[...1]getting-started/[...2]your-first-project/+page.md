# gama cli

Gama cli tool is what helps you to create, build, package
and distribute projects with gama, it is built in V language
and comes with prebuilt binaries for each platform which are
copied to your project when it's created.

## Creating projects

To create a project you will use the `gama create` command, 
the assistant will ask you a few questions and then create
the project.
- ```
  ╰─>$ gama create
  Welcome to gama project creation assistant, let me load a few things
  Loaded installation with gama version 0.1.0 and 3 templates
  Enter the project name, the project name should only contain lowercase letters, numbers, and underscores
  > 
  ```
  Type in the name of your project, only letters, numbers and
  underscores are allowed, gama will automatically remove
  unallowed characters.
  ```
  > my project name
   Some characters had to be removed
  The project will be named: myprojectname is that okay? (Yep / nop)?
  ```
- ```
  Oh sounds cool, what is myprojectname about?
  >
  ```
  Write a short description of your project
  ```
  > It is a simple project
  ```
- ```
  And what do you want to use as template for your application?
  0) pong  A simple pong game down with gama physics
  1) skeleton  A barebone gama project ready to receive dev flesh
  > 
  ```
  Chose a template for creating your project, templates are gama
  projects with already working boilerplate code, so you don't 
  have to start from nowhere, it will ask you for confirmation.
  ```
  > 1
  so we use template skeleton? (Yep/nop)
  ```
```

And that's it, your project's generated.

## Developing and Building Projects

Gama offers a streamlined workflow for both rapid development and optimized release builds.

### `gama dev` for Rapid Development

This is your go-to command for active development. It uses the fast `tcc` compiler to quickly compile and run your project.
Crucially, `gama dev` also supports **hot-reloading**: any changes you save in your project will automatically trigger a recompile and restart of your game, providing immediate feedback.

```
Usage: gama dev [flags]

Runs the current gama project in development mode with hot-reloading.

Flags:
  -cc  -cc            Use an alternative compiler (e.g., 'clang', 'tcc', 'zig')
       -help          Prints help information.
       -man           Prints the auto-generated manpage.
```

### `gama build` for Release Builds

When you are ready to create a final, optimized executable for distribution, use `gama build`. This command typically leverages the more powerful `zig cc` compiler for advanced optimizations.

```
Usage: gama build [flags]

Builds the current gama project for release.

Flags:
  -r   -run           Run the project after building
  -cc  -cc            Use an alternative compiler
       -help          Prints help information.
       -man           Prints the auto-generated manpage.
```

### `gama bake` for Asset Embedding

The `gama bake` command allows you to embed assets (like images, sounds, or 3D models) directly into your game's executable. This can simplify distribution and reduce loading times.

```
Usage: gama bake [flags]

Embeds project assets directly into the executable.

Flags:
  -c   -c             Use compression for embedded assets
       -help          Prints help information.
       -man           Prints the auto-generated manpage.
```

### `gama update` for Refreshing Project Files

The `gama update` command refreshes your project's support files from your current Gama installation. It copies the latest C header files and build system resources into your project's `include/` and `build/` directories.

This is very useful after you have updated Gama to a new version, as it ensures your existing projects can use the latest features and fixes without needing to be recreated.

```
Usage: gama update [flags]

Updates the project's headers and build files from the Gama installation.

Flags:
       -help          Prints help information.
       -man           Prints the auto-generated manpage.
```

### TIP

To build and run your project directly, you can use `gama build -r`.
