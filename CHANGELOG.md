# gama changelog

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
