module gama

#flag -I @VMODROOT/../lib
#flag -D GM_GEN
#flag -D GM_NO_GAPI

#include <gama.h>
#include <gama/debug.h>
#include <gama/3d.h>

@[typedef]
struct C.gmImageData {
	width  i32
	height i32
	data   &u8
}

pub fn C.gm_image_data_load(data &C.gmImageData, path &char) i32
pub fn C.gm_image_data_free(data &C.gmImageData) i32

pub fn load_image(path string) !C.gmImageData {
	data := C.gmImageData{}
	ret := C.gm_image_load(&data, &char(path.str))
	return data
}

@[typedef]
struct C.gm3Mesh {}

@[typedef]
struct C.gmStr {
	length  usize
	content &char
}

pub fn C.gm_str() C.gmStr

pub fn C.gm3_obj_load(mesh &C.gm3Mesh, path &char, mtl_dir &char) i32

pub fn C.gmg_mesh(str &C.gmStr, m C.gm3Mesh) i32

@[unsafe]
pub fn load_mesh(path string, mtldir string) !C.gm3Mesh {
	mesh := C.gm3Mesh{}
	ret := C.gm3_obj_load(&mesh, &char(path.str), &char(mtldir.str))
	if ret < 0 {
		return error('COuld not load ${path}, obj_load exited with code ${ret}')
	}
	return mesh
}

@[unsafe]
pub fn generate_mesh(mesh C.gm3Mesh) !string {
	str := C.gm_str()
	ret := C.gmg_mesh(&str, mesh)
	if ret < 0 {
		return error('Could not generate mesh  gmg_mesh exited with code ${ret}')
	}
	return str.content.vstring()
}
