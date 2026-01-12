# Creating 3d mesh files

There exists several formats for saving 3d files, such as OBJ, FBX, and glTF. Each format has its own advantages and disadvantages, and the choice of format depends on the specific requirements of your project, for convenience, gama
has support for both the OBJ(wavefront) and the glTF formats.

The obj format is a simple text-based format that is easy to read and write. It is commonly used for 3d modeling and animation, and is supported by many 3d modeling software. The glTF format is a binary format that is optimized for web use. It is commonly used for 3d modeling and animation, and is supported by many 3d modeling software.

On the other hand, the glTF format is a binary format that is optimized for web use. It is commonly used for 3d modeling and animation, and is supported by many 3d modeling software, it integrates seamlessly all the object textures into a single file, which makes it easier to load and display.

To make the app and assets distributable to the web, you are advised to make use of the baking system to include the files directly in the app.

To get started with creating 3d mesh files, you can use a variety of tools and software, such as Blender, Maya, or 3ds Max. These tools allow you to create and edit 3d models, and export them in the desired format, you can also search for free 3d models online but remember that gama supports the glTF and obj formats only.

## Note for obj users

An obj model may depend on several files:
- an obj file containing objects
- a mtl file containing materials
- one or more texture files containing textures
Gama handles best the two first and has basic support for textures.
