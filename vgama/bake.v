module vgama

import os
import gama
import term

@[unsafe]
pub fn bake_obj(path string, fname string) !string {
	mesh := gama.load_mesh(path, os.dir(path))!

	code := gama.generate_mesh(mesh)!

	return '
#include <gama/3d/mesh.h>
#include <gama/3d/mtl.h>

// Baked image data for ${os.file_name(path)}

const gm3Mesh _${fname}_data;
static inline gm3Mesh ${fname}();

//////////

const gm3Mesh _${fname}_data = ${code};

gm3Mesh ${fname}() {
	return _${fname}_data;
}'
}

pub fn bake_img(path string, fname string) !string {
	bytes := os.read_bytes(path)!

	mut byte_str := ''
	for i, b in bytes {
		byte_str += '0x${b.hex()}, '
		if (i + 1) % 16 == 0 {
			byte_str += '\n\t'
		}
	}

	return '
#include <gama/image.h>

// Baked image data for ${os.file_name(path)}

static const unsigned char _${fname}_data[];
static const unsigned int _${fname}_len;
static inline gmImage ${fname}();

//////////


static const unsigned int _${fname}_len = ${bytes.len};
static const unsigned char _${fname}_data[] = {
	${byte_str}
};
gmImage ${fname}() {
	return gm_image_create_from_memory(_${fname}_data, _${fname}_len);
}
'
}

pub fn (p Project) bake(inst Installation) ! {
	assets_dir := os.join_path(p.path, 'assets')
	gen_dir := os.join_path(p.build_path('gen'), 'assets')

	os.walk(assets_dir, fn [assets_dir, gen_dir] (path string) {
		if path.starts_with(assets_dir) {
			relpath := path[assets_dir.len + 1..]
			fname := os.file_name(path)
			dest_h := os.join_path(gen_dir, relpath + '.h')
			// dest_c := os.join_path(gen_dir, relpath + '.c')
			if !should_build_to(path, dest_h) {
				return
			}
			mut content := ''
			if relpath.ends_with('.obj') {
				println(' - baking ${relpath}')
				var := fname[0..fname.len - 4].replace('.', '_') + '_mesh'
				unsafe {
					content = bake_obj(path, var) or {
						println(term.fail_message('${err}'))
						return
					}
				}
			} else if relpath.ends_with('.png') || relpath.ends_with('.jpg')
				|| relpath.ends_with('.jpeg') || relpath.ends_with('.bmp') {
				println(' - baking ${relpath}')
				// Generate a C-style variable name from the file name
				ext_len := os.file_ext(relpath).len
				base_name := fname[0..fname.len - ext_len]
				var := base_name.replace('-', '_').replace('.', '_') + '_image'
				content = bake_img(path, var) or {
					println(term.fail_message('${err}'))
					return
				}
			} else {
				return
			}
			os.mkdir_all(os.dir(dest_h)) or {
				println(term.fail_message('Failed to create parent dir for ${dest_h}'))
				return
			}
			if content != '' {
				os.write_file(dest_h, content) or {
					println(term.fail_message('Failed to write to ${dest_h}'))
				}
			}
		}
	})
}

fn should_build_to(orig string, dest string) bool {
	return if os.exists(dest) {
		sorig := os.stat(orig) or { return true }
		sdest := os.stat(dest) or { return true }
		sorig.mtime > sdest.mtime
	} else {
		true
	}
}
