# Loading 3D Mesh Files

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
