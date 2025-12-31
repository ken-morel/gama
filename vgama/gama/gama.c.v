module gama

#flag -I @VMODROOT/../lib
#flag -D GM_GEN
#flag -D GM_NO_GAPI

#include <gama.h>
#include <gama/debug.h>
#include <gama/3d.h>

@[typedef]
struct C.gm3Mesh {}

@[typedef]
struct C.gmStr {
	length  usize
	content &char
}

pub fn C.gm3_obj_load(mesh &C.gm3Mesh, path &char, mtl_dir &char) i32

pub fn C.gmg_mesh(m C.gm3Mesh) C.gmStr

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
pub fn generate_mesh(mesh C.gm3Mesh) string {
	str := C.gmg_mesh(mesh)
	return str.content.vstring()
}
