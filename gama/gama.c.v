module gama

#flag -I @VMODROOT/../lib
#flag -D GM_GEN
#flag -D GM_NO_GAPI

#include <gama.h>
#include <gama/debug.h>
#include <gama/3d.h>
#include <stdlib.h> // For C.free
#include <gama/3d/gltf.h> // For gltf loader and mesh serialization

pub fn C.gm3_gltf_load(mesh &C.gm3Mesh, path &char) i32

pub fn C.gm3_mesh_deserialize(mesh &C.gm3Mesh, data &&C.void, size u64) i32
pub fn C.gm3_mesh_serialize(mesh &C.gm3Mesh, data &C.void, size &u64) i32
pub fn C.gm3_mesh_free(m &C.gm3Mesh)

@[typedef]
struct C.gmImageData {
	width  i32
	height i32
	data   &u8
}

pub fn C.gm_image_data_load(data &C.gmImageData, path &char) i32
pub fn C.gm_image_data_free(data &C.gmImageData) i32

pub fn load_image(path string) !C.gmImageData {
	data := C.gmImageData{
		width:  0
		height: 0
		data:   unsafe { nil }
	}
	ret := C.gm_image_data_load(&data, &char(path.str))
	if ret < 0 {
		return error('Image load returned negative status ${ret}')
	}
	return data
}

@[typedef]
struct C.gm3Mesh {}

@[typedef]
struct C.gmStr {
	length  usize
	content &char = unsafe { nil }
}

pub fn C.gm_str() C.gmStr

pub fn C.gm3_obj_load(mesh &C.gm3Mesh, path &char, mtl_dir &char) i32

@[unsafe]
pub fn load_mesh(path string, mtldir string) !C.gm3Mesh {
	mesh := C.gm3Mesh{}
	ret := C.gm3_obj_load(&mesh, &char(path.str), &char(mtldir.str))
	if ret < 0 {
		return error('COuld not load ${path}, obj_load exited with code ${ret}')
	}
	return mesh
}
