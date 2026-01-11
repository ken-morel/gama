module vgama

import os
import gama
import term

@[unsafe]
pub fn bake_obj(path string, fname string) !string {
	mesh := gama.load_mesh(path, os.dir(path))!

	code := gama.generate_mesh(mesh)!

	return '
#pragma once

#include <gama/3d/mesh.h>
#include <gama/3d/mtl.h>

// Baked mesh from ${os.file_name(path)}

const gm3Mesh _${fname}_data;
static inline gm3Mesh ${fname}();

//////////

const gm3Mesh _${fname}_data = ${code};

static inline gm3Mesh ${fname}() {
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
#pragma once
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
static inline gmImage ${fname}() {
	return gm_image_create_from_memory(_${fname}_data, _${fname}_len);
}
'
}

// format_size converts a byte count into a human-readable string.
fn format_size(bytes i64) string {
	if bytes < 1024 {
		return '${bytes} B'
	}
	mut val := f64(bytes) / 1024.0
	if val < 1024 {
		return '${val} KB'
	}
	val /= 1024.0
	if val < 1024 {
		return '${val} MB'
	}
	val /= 1024.0
	return '${val} GB'
}

pub fn (p Project) bake(inst Installation, clean bool) ! {
	assets_dir := os.join_path(p.path, 'assets')
	gen_dir := os.join_path(p.build_path('gen'), 'assets')
	if clean {
		os.rmdir_all(gen_dir) or {
			println(term.warn_message('Error deleting gen directory: ${err}'))
		}
	}

	os.walk(assets_dir, fn [p, assets_dir, gen_dir] (path string) {
		if path.starts_with(assets_dir) {
			relpath := path[assets_dir.len + 1..]
			fname := os.file_name(path)
			dest_h := os.join_path(gen_dir, relpath + '.h')

			if !should_build_to(path, dest_h) {
				return
			}

			// First, figure out if we have a handler for this file type.
			mut content := ''
			if relpath.ends_with('.obj') {
				var := fname[0..fname.len - 4].replace('.', '_') + '_mesh'
				unsafe {
					content = bake_obj(path, var) or {
						println(term.fail_message('${err}'))
						return
					}
				}
			} else if relpath.ends_with('.png') || relpath.ends_with('.jpg')
				|| relpath.ends_with('.jpeg') || relpath.ends_with('.bmp') {
				ext_len := os.file_ext(relpath).len
				base_name := fname[0..fname.len - ext_len]
				var := base_name.replace('-', '_').replace('.', '_') + '_image'
				content = bake_img(path, var) or {
					println(term.fail_message('${err}'))
					return
				}
			} else {
				// If no handler, skip the file.
				return
			}

			// If content was generated, log it and write the file.
			if content != '' {
				file_type := os.file_ext(path)[1..].to_upper()
				relative_input := os.join_path('assets', relpath)
				relative_output := dest_h[p.path.len + 1..]
				file_size_bytes := os.stat(path) or { return }.size
				file_size_str := format_size(file_size_bytes)

				print(term.bright_blue('Baking '))
				print(term.bold('[${file_type}] '))
				print('${relative_input} -> ${relative_output} ')
				println(term.gray('(${file_size_str})'))

				os.mkdir_all(os.dir(dest_h)) or {
					println(term.fail_message('Failed to create parent dir for ${dest_h}'))
					return
				}
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
