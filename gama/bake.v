module gama

import os
import term
import time

@[unsafe]
fn bake_mesh_data(mesh C.gm3Mesh, path string, fname string) !string {
	// 1. Serialize mesh in C
	mut data_ptr := &u8(unsafe { nil }) // Correct Vlang syntax for C pointer
	mut data_size := u64(0)
	if C.gm3_mesh_serialize(&mesh, &data_ptr, &data_size) != 0 {
		return error('Failed  to serialize mesh from: ${path}')
	}
	defer {
		C.free(data_ptr) // Freeing C-allocated memory using C.free
	}

	// 2. Format bytes into C array string by directly indexing the C pointer
	mut byte_str := ''

	for i in 0 .. data_size {
		b := data_ptr[i]
		byte_str += '0x${b.hex()}, '
		if (i + 1) % 16 == 0 {
			byte_str += '\n\t'
		}
	}

	// 3. Return the generated code
	return '
#pragma once
#include <gama/3d/mesh.h> // Contains the deserialization function

// Baked mesh data for ${os.file_name(path)}
static const unsigned char _${fname}_data[];
static const unsigned int _${fname}_len;
static inline gm3Mesh ${fname}();
//////////
static const unsigned int _${fname}_len = ${data_size};
static const unsigned char _${fname}_data[] = {
	${byte_str}
};
static inline gm3Mesh ${fname}() {
	gm3Mesh mesh;
	gm3_mesh_deserialize(&mesh, _${fname}_data, _${fname}_len);
	return mesh;
}
'
}

@[unsafe]
pub fn bake_obj(path string, fname string) !string {
	mut mesh := C.gm3Mesh{}
	if C.gm3_obj_load(&mesh, path.str, os.dir(path).str) != 0 {
		return error('Failed to load OBJ file: ${path}')
	}
	defer {
		C.gm3_mesh_free(&mesh)
	}
	return bake_mesh_data(mesh, path, fname)
}

@[unsafe]
pub fn bake_gltf(path string, fname string) !string {
	mut mesh := C.gm3Mesh{}
	if C.gm3_gltf_load(&mesh, path.str) != 0 {
		return error('Failed to load GLTF file: ${path}')
	}
	defer {
		C.gm3_mesh_free(&mesh)
	}
	return bake_mesh_data(mesh, path, fname)
}

pub fn bake_img(path string, fname string) !string {
	bytes := os.read_bytes(path)!

	mut byte_str := ''
	for i in 0 .. bytes.len {
		b := bytes[i]
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
		return val.str() + ' KB'
	}
	val /= 1024.0
	if val < 1024 {
		return val.str() + ' MB'
	}
	val /= 1024.0
	return val.str() + ' GB'
}

struct AssetHandler {
	kind       string   // e.g. 'Model', 'Image'
	scan_dir   string   // e.g. 'assets/gltf'
	extensions []string // e.g. ['.gltf', '.glb']
}

struct AssetToBake {
	handler_kind string
	src_path     string
	dest_path    string
}

// get_baker returns the correct baking function for a given file extension
fn get_baker(ext string) (fn (string, string) !string, string) {
	match ext {
		'.obj' { return bake_obj, '_mesh' }
		'.gltf', '.glb' { return bake_gltf, '_mesh' }
		'.png', '.jpg', '.jpeg', '.bmp' { return bake_img, '_image' }
		else { panic('No baker for extension: ${ext}') }
	}
}

pub fn (p Project) bake(inst Installation, clean bool) ! {
	assets_dir := os.join_path(p.path, 'assets')
	gen_dir := os.join_path(p.build_path('gen'), 'assets')
	if clean && os.exists(gen_dir) {
		os.rmdir_all(gen_dir) or {
			println(term.warn_message('Error deleting gen directory: ${err}'))
		}
	}

	handlers := [
		AssetHandler{
			kind:       'Model'
			scan_dir:   'gltf'
			extensions: ['.gltf', '.glb']
		},
		AssetHandler{
			kind:       'Model'
			scan_dir:   'obj'
			extensions: ['.obj']
		},
		AssetHandler{
			kind:       'Image'
			scan_dir:   'images'
			extensions: ['.png', '.jpg', '.jpeg', '.bmp']
		},
	]

	mut files_to_bake := []AssetToBake{}

	// --- Pass 1: Collect files that need to be baked ---
	for handler in handlers {
		scan_path := os.join_path(assets_dir, handler.scan_dir)
		if !os.exists(scan_path) {
			continue
		}
		files := os.ls(scan_path) or { continue }
		for file in files {
			for ext in handler.extensions {
				if file.ends_with(ext) {
					src_path := os.join_path(scan_path, file)
					rel_path := os.join_path(handler.scan_dir, file)
					dest_path := os.join_path(gen_dir, rel_path + '.h')
					if should_build_to(src_path, dest_path) {
						files_to_bake << AssetToBake{handler.kind, src_path, dest_path}
					}
					break
				}
			}
		}
	}

	if files_to_bake.len == 0 {
		println(term.bright_green('All assets up to date.'))
		return
	}

	// --- Pass 2: Bake the collected files ---
	println('Baking ${files_to_bake.len} assets...')
	mut baked_count := 0
	start_time := time.ticks()

	for asset in files_to_bake {
		baked_count++
		percentage := (baked_count * 100) / files_to_bake.len
		progress_bar := '[${percentage:3}%] Baking ${asset.handler_kind}: ${os.file_name(asset.src_path)}'
		print('\r' + left_justify(progress_bar, 80, ' '))
		mut stdout := os.stdout()
		stdout.flush()

		ext := os.file_ext(asset.src_path)
		baker, suffix := get_baker(ext)

		base_name := os.file_name(asset.src_path)[0..os.file_name(asset.src_path).len - ext.len]
		var_name := base_name.replace('-', '_').replace('.', '_') + suffix

		content := unsafe {
			baker(asset.src_path, var_name) or {
				println('\n' + term.fail_message('Failed to bake ${asset.src_path}: ${err}'))
				continue
			}
		}

		os.mkdir_all(os.dir(asset.dest_path)) or {
			println('\n' + term.fail_message('Failed to create parent dir for ${asset.dest_path}'))
			continue
		}
		os.write_file(asset.dest_path, content) or {
			println('\n' + term.fail_message('Failed to write to ${asset.dest_path}'))
		}
	}

	end_time := time.ticks()
	duration_ms := end_time - start_time
	println('\n' +
		term.bright_green('Finished baking ${files_to_bake.len} assets in ${duration_ms}ms.'))
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

fn left_justify(txt string, n i32, c string) string {
	mut newtxt := txt
	for _ in txt.len .. n {
		newtxt += c
	}
	return newtxt
}
