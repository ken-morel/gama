# gama changelog

## v0.1.1-dev5

- You can now use `gama create 'project name'` Thanks to a little trick
- Added `gama clean` and fixed `gama update` commands.
- Added support for backing normal files.

## v0.1.1-dev4 || v0.1.1-rc1

- Packaging support for arch linux.
- **New compiler api**: the compilées powering gama can be accessed via `gama tcc` , `zig` and `zcc` , where `zcc` is `zig cc`.  this permits you to integrate gama compilées in your workflow especially on windows where it's quite useful.

- **Integrated Compiler Toolchain**: Reworked the entire build process to improve developer experience and portability.
  - **Bundled Compilers (Windows)**: The Windows installer now comes with pre-configured **TCC** and **Zig** compilers, providing a true "out-of-the-box" development environment with no external dependencies required.
  - **System Dependencies (Linux)**: The Linux packages (`.deb`, `.pkg.tar.zst`) now declare `tcc` and `zig` as external dependencies, leveraging the system's package manager for a smaller footprint and native integration.
  - **New `gama dev` command**: A new command for rapid, iterative development. It uses the fast **TCC** compiler by default and features hot-reloading on file changes.
  - **Updated `gama build` command**: The `build` command is now focused on release builds, using the more powerful and optimizing **Zig CC** compiler by default.
- **Improved Project Portability**: Removed the `compiler` field from `gama.toml`, making project files toolchain-agnostic and easier to share between different systems.

- Gama headers are located to /usr/lib/gama on linux

## v0.1.1-dev3

- Started the changelog.
- First class support for web builds, via the new `gama.js`, with a complete
  remake of gapi in typescript, optimised with the same loop as in native
  builds thanks to channel messages, `SharedArrayBuffer` and operations
  on them permiting synchronization. Building with `zig cc` and providing
  a basic wasi polifill which does not permit more than accessing
  `stdin`, `stdout` and `stderr`. And touch/keyboard support.
- Created a new though minimalistic 3d engine in gama, with support for
  lighting, transforms, and loading `obj` and `gltf` files. The projection
  is done with the new `gm_draw_triangles` function and thus calculations
  for projection are done at a per-face level.
- Performance improvements in libvgama, including better stacking of draw
  calls resulting in less channel push overhead. Fixed application closing bug which
  kept the shell open.
- Gama projects now get each attributed a uuid v7 on creation, to help
  for project identification purposes.
- New baking system implemented using gama itself, permiting
  to integrate compressed serialized game assets into app executable.
- Centralized build pipelines to a new `mng` v script at project root.

## v0.1.1-dev2

- Added doxygen documentation to gama, ready to build to html. Created
  a new site for gama with basic homepage.
- Implemented gama widgets from basic shapes primitives
- Added support for sprites.

## v0.1.1-dev1

- **Switch from go to V**

- Created gama physics system, with persistent collisions, support for
  static and immobile objects.
- Better editor support with `.clangd`, `.cbp` and `.sublime-project`
  files.
