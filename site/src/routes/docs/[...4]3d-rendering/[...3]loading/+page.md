# Loading 3D Mesh Files

View:
- <a href="/reference/obj_8h.html">gama/3d/obj.h reference</a>
- <a href="/reference/gltf_8h.html">gama/3d/gltf.h reference</a>

There are two ways of using 3d mesh files in your apps, you
can either load the files at runtime(works for native builds only)
which will load the data when your app is running, or you can bake
them, producing source files you can directly include in your project,
it reduces load time.
To support web platforms, you are advice to use the second method.

## Loading at Runtime

```c
gm3Mesh mesh;

int status = gm3_obj_load(&mesh, "path/to/your/model.obj", "path/to/materials/folder");

status = gm3_gltf_load(&mesh, "path/to/your/model.gltf");

if(status < 0) {
    // error loading the mesh
}
```

The loader functions will return a status code indicating the success or failure of the operation.
A status code of 0 indicates success, a status higher than 0 indicates a warning and a status lower than 0 indicates an error.

## Loading from baked data

At the root of your project folder lies an `assets` folder, which is expected to hold your
game assets, including sprites, images, sound and 3d objects, if you place an obj
file in `assets/obj` or a gltf file in `assets/gltf` i.e you create subfolders in the `assets`
folder, then when you build your application, gama will automatically generate `.h` files
which you can incude in your app.

Create a file at `assets/obj/cube.obj`, and in that file insert this:
```python
# Simple Cube with Vertices and Normals
o Cube

# Vertices (8)
v -1.0 -1.0  1.0
v  1.0 -1.0  1.0
v  1.0  1.0  1.0
v -1.0  1.0  1.0
v -1.0 -1.0 -1.0
v  1.0 -1.0 -1.0
v  1.0  1.0 -1.0
v -1.0  1.0 -1.0

# Normals (6, one for each face)
vn  0.0  0.0  1.0
vn  0.0  0.0 -1.0
vn  0.0  1.0  0.0
vn  0.0 -1.0  0.0
vn  1.0  0.0  0.0
vn -1.0  0.0  0.0

# Faces (12 triangles)
# Format: f vertex_index//normal_index
f 1//1 2//1 3//1
f 1//1 3//1 4//1
f 5//2 8//2 7//2
f 5//2 7//2 6//2
f 4//3 3//3 7//3
f 4//3 7//3 8//3
f 1//4 5//4 6//4
f 1//4 6//4 2//4
f 2//5 6//5 7//5
f 2//5 7//5 3//5
f 1//6 4//6 8//6
f 1//6 8//6 5//6
```

Then you run `gama bake` to generate a c header, `gama bake -c` will delete
the files and regenerate the header. You can then include and use the file:

```c
#include <assets/obj/cube.obj.h> // notice the .h !!

int main(){
    const gm3Mesh cube = cube_mesh(); // load the mesh
}
```
