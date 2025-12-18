## Development

### Using gama cli

With a configured compiler and gama cli installed you can easily build the
project.

Install gama cli from [gama documentation](https://gama.rbs.cm/installation).

Open a terminal in your project folder and use:
- `gama build -r`: To build and run the project.
- `gama dev`: To rebuild and rerun the project anytime a file changes.
- `gama package`: To package the project executable.

Note that in case you fall on an error compiling, it may simply be that
you need to configure your own compiler, simply add `-cc compiler-path`
to tell gama to use that compiler instead.

The sublime text project files have ready to use build commands which
build the project acording to those, but you may configure it as you wish.

### Codeblocks

Gama projects currently also bind to code blocks build system, meaning
you can build and open a gama project with codeblocks using codeblock's
compiler even without gama installed(since projects contain all required
project files).
