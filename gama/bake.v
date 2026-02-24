module gama

import os
import term
import time
import strings

const bkmsg = 'Baked data has invalid size. Sorry. Try building again and contacting gama'

@[unsafe]
fn generate_c_bytearray(data_ptr &u8, data_size u64) string {
	mut byte_str := strings.new_builder(int(data_size) * 7)
	mut p1k := data_size / 1000
	if p1k < 10 {
		p1k = 10
	}
	byte_str.write_string('"')
	mut llen := 0

	for i in 0 .. data_size {
		b := data_ptr[i]
		if false && b >= 32 && b <= 126 {
			byte_str.write_byte(b)
			llen++
		} else {
			// convert to u8 to have 0 padding
			byte_str.write_string('\\x${b.hex()}')
			llen += 4
		}
		if llen >= 100 {
			byte_str.write_string('"\n\t"')
			llen = 0
		}
		if i != 0 && i % p1k == 0 {
			percent := (f64(i) / f64(data_size)) * f64(100)
			print('\r[${percent:5}%]')
		}
	}
	byte_str.write_string('"')
	print('\r[100.00%]')
	return byte_str.str()
}

@[unsafe]
fn bake_mesh_data(mesh C.gm3Mesh, path string, fname string) !string {
	flag := 'GM_ASSET_MESH_${fname.to_upper()}_INCLUDED'

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
	byte_str := generate_c_bytearray(data_ptr, u64(data_size))

	// 3. Return the generated code
	return '
#ifndef ${flag}
#define ${flag}

#include <gama/3d/mesh.h> // Contains the deserialization function
#include <gama/assert.h>

// Baked mesh data for ${os.file_name(path)}
static const unsigned char _${fname}_data[];
static const unsigned int _${fname}_len;
static inline gm3Mesh ${fname}();
//////////
static const unsigned int _${fname}_len = ${data_size};
static const unsigned char _${fname}_data[${data_size}] = ${byte_str};
gm3Mesh _${fname}_mesh;

gm_static_assert(sizeof(_${fname}_data) == _${fname}_len + 1, "${bkmsg}");
static inline gm3Mesh ${fname}() {
	static int loaded = 0;
	if(!loaded) {
		gm3_mesh_deserialize(&_${fname}_mesh, _${fname}_data, _${fname}_len);
		loaded = 1;
	}
	return _${fname}_mesh;
}
#endif // ${flag}
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

@[unsafe]
pub fn bake_img(path string, fname string) !string {
	bytes := os.read_bytes(path)!

	byte_str := generate_c_bytearray(bytes.data, u64(bytes.len))

	flag := 'GM_ASSET_IMAGE_${fname.to_upper()}_INCLUDED'

	return '
#ifndef ${flag}
#define ${flag}
#include <gama/image.h>
#include <gama/assert.h>

// Baked image data for ${os.file_name(path)}
static const unsigned char _${fname}_data[];
static const unsigned int _${fname}_len;
static inline gmImage ${fname}();
//////////
static const unsigned int _${fname}_len = ${bytes.len};
static const unsigned char _${fname}_data[] = ${byte_str};
gmImage ${fname}_image;
gm_static_assert(sizeof(_${fname}_data) == _${fname}_len + 1, "${bkmsg}");
static inline gmImage ${fname}() {
	static int loaded = 0;
	if(!loaded) {
		${fname}_image = gm_image_create_from_memory(_${fname}_data, _${fname}_len);
		loaded = 1;
	}
	return ${fname}_image;
}

#endif // ${flag}
'
}

@[unsafe]
pub fn bake_sound(path string, fname string) !string {
	bytes := os.read_bytes(path)!

	data := C.gm_compress(bytes.data, bytes.len)
	flag := 'GM_ASSET_SOUND_${fname.to_upper()}_INCLUDED'

	byte_str := generate_c_bytearray(data.data, u64(data.compressed))
	defer {
		C.gm_compressed_free(data)
	}

	return '
#ifndef ${flag}
#define ${flag}

#include <gama/compress.h>
#include <gama/sound.h>
#include <gama/assert.h>
#include <gama/log.h>


static const unsigned char _${fname}_data_compressed[] = ${byte_str};
static unsigned char _${fname}_data[${data.original}] = {0};
gm_static_assert(sizeof(_${fname}_data_compressed) == ${data.compressed} + 1, "${bkmsg}");
static gmSound ${fname}_sound = {0};

static inline gmSound ${fname}() {
	static int loaded = 0;
	if(!loaded) {
		gm_decompress_to(_${fname}_data_compressed, ${data.compressed}, _${fname}_data, ${data.original});
		if (gm_load_sound_from_memory(&${fname}_sound, _${fname}_data, ${data.original}))
		    gm_log_error("Error loading audio");
	}
	return ${fname}_sound;
}

#endif // ${flag}
'
}

@[unsafe]
pub fn bake_data(path string, fname string) !string {
	bytes := os.read_bytes(path)!

	data := C.gm_compress(bytes.data, bytes.len)
	flag := 'GM_ASSET_DATA_${fname.to_upper()}_INCLUDED'

	byte_str := generate_c_bytearray(data.data, u64(data.compressed))
	defer {
		C.gm_compressed_free(data)
	}

	return '
#ifndef ${flag}
#define ${flag}

#include <gama/compress.h>
#include <gama/assert.h>


static const unsigned char _${fname}_data_compressed[] = ${byte_str};

static unsigned char _${fname}_data[${data.original}] = {0};

gm_static_assert(sizeof(_${fname}_data_compressed) == ${data.compressed} + 1, "${bkmsg}");
static inline unsigned char* ${fname}(size_t* size) {
	static int decompressed = 0;
	*size = ${data.original};
	if(!decompressed) {
		gm_decompress_to(_${fname}_data_compressed, ${data.compressed}, _${fname}_data, ${data.original});
		decompressed = 1;
	}
	return _${fname}_data;
}


#endif // ${flag}
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

@[heap]
struct AssetHandler {
	kind       string
	scan_dir   string
	extensions []string
	handler    fn (string, string) !string @[required]
	suffix     string
}

struct AssetToBake {
	handler_kind string
	src_path     string
	dest_path    string
	handler      AssetHandler
}

fn (a AssetHandler) handles_file(file string) bool {
	if a.extensions.len == 0 {
		return true
	}
	for ext in a.extensions {
		if file.ends_with(ext) {
			return true
		}
	}
	return false
}

const asset_handlers = [
	AssetHandler{
		kind:       'Model'
		suffix:     '_mesh'
		scan_dir:   'gltf'
		extensions: ['.gltf', '.glb']
		handler:    bake_gltf
	},
	AssetHandler{
		kind:       'Model'
		suffix:     '_mesh'
		scan_dir:   'obj'
		extensions: ['.obj']
		handler:    bake_obj
	},
	AssetHandler{
		kind:       'Sound'
		suffix:     '_sound'
		scan_dir:   'sounds'
		extensions: ['.wav', '.mp3', '.ogg']
		handler:    bake_sound
	},
	AssetHandler{
		kind:       'Image'
		suffix:     '_image'
		scan_dir:   'images'
		extensions: ['.png', '.jpg', '.jpeg', '.bmp']
		handler:    bake_img
	},
	AssetHandler{
		kind:       'Data'
		suffix:     '_data'
		scan_dir:   'data'
		extensions: []
		handler:    bake_data
	},
]

pub fn (p Project) bake(inst Installation, clean bool) ! {
	assets_dir := os.join_path(p.path, 'assets')
	gen_dir := os.join_path(p.build_path('gen'), 'assets')
	if clean && os.exists(gen_dir) {
		os.rmdir_all(gen_dir) or {
			println(term.warn_message('Error deleting gen directory: ${err}'))
		}
	}

	mut files_to_bake := []AssetToBake{}

	// --- Pass 1: Collect files that need to be baked ---
	for handler in asset_handlers {
		scan_path := os.join_path(assets_dir, handler.scan_dir)
		if !os.exists(scan_path) {
			continue
		}
		files := os.ls(scan_path) or { continue }
		for file in files {
			if handler.handles_file(file) {
				src_path := os.join_path(scan_path, file)
				rel_path := os.join_path(handler.scan_dir, file)
				dest_path := os.join_path(gen_dir, rel_path + '.h')
				if should_build_to(src_path, dest_path) {
					files_to_bake << AssetToBake{handler.kind, src_path, dest_path, handler}
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
		progress_bar := '[00.00%] [${percentage:3}%] Baking ${asset.handler_kind}: ${os.file_name(asset.src_path)}'
		print('\r' + left_justify(progress_bar, 80, ' '))
		mut stdout := os.stdout()
		stdout.flush()

		ext := os.file_ext(asset.src_path)
		baker := asset.handler.handler
		suffix := asset.handler.suffix

		base_name := os.file_name(asset.src_path)[0..os.file_name(asset.src_path).len - ext.len]
		var_name := base_name.replace('-', '_').replace('.', '_').replace(' ', '_') + suffix

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
