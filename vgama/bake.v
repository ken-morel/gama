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

const gm3Mesh _${fname}_data = ${code};

static inline gm3Mesh ${fname}() {
	return _${fname}_data;
}'
}

pub fn (p Project) bake(inst Installation) ! {
	assets_dir := os.join_path(p.path, 'assets')
	gen_dir := os.join_path(p.build_path('gen'), 'assets')

	os.walk(assets_dir, fn [assets_dir, gen_dir] (path string) {
		if path.starts_with(assets_dir) {
			relpath := path[assets_dir.len + 1..]
			fname := os.file_name(path)
			dest_h := os.join_path(gen_dir, relpath + '.h')
			if !should_build_to(path, dest_h) {
				return
			}
			mut content := ''
			if relpath.ends_with('.obj') {
				println(' - baking ${relpath}')
				var := fname[0..fname.len - 4] + '_mesh'
				unsafe {
					content = bake_obj(path, var) or {
						println(term.fail_message('${err}'))
						return
					}
				}
			} else {
				return
			}
			os.mkdir_all(os.dir(dest_h)) or {
				println(term.fail_message('Failed to create parent dir for ${dest_h}'))
				return
			}
			os.write_file(dest_h, content) or {
				println(term.fail_message('Failed to write to ${dest_h}'))
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
