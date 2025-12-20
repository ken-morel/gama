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
- ```
  Looking for compilers...
  Please choose a compiler, you can still change it latter
  0) no compiler
  1) clang at /usr/bin/clang  A relatively fast compiler for modern systems
  2) gcc at /usr/bin/gcc  Robust compiler(comes with code blocks)
  3) tcc at /usr/bin/tcc  Tiny and extremely fast compiler
  > 
  ```
  Notice the output will surely differ on your machine, you are asked to choose
  a default compier for your project.

  ```
  so we use compiler Compiler{
      name: 'clang'
      path: '/usr/bin/clang'
  }? (Yep/nop)
  myprojectname generated successfuly!
  ```

And that's it, your project's generated.

## Building projects

To build a gama project, you can use `gama build` command, it builds
your gama project using `vgama` backend and creates an executable at
`build/native`, in the same folder there will be a file named `libvgama`.
Which permits your c application to create a window and draw graphics, so
is of the utmost importance.

```
Usage: gama build [flags] build [-r] [-cc name]

Builds the current gama project

Flags:
  -r   -run           Run the project after building
  -cc  -cc            Use an alternative compiler
       -help          Prints help information.
       -man           Prints the auto-generated manpage.
```

### TIP

To build and run your project directly, you can use `gama build -r`.

More on the gama cli will be covered in the guides section.
