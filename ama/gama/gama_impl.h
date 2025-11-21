#ifndef gama_impl_included
#define gama_impl_included

#define SOKOL_IMPL
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#define STB_IMAGE_RESIZE2_IMPLEMENTATION

// Custom memory management callbacks for STB libraries
#include <stddef.h>
extern void * stbi__callback_malloc(size_t size);
extern void * stbi__callback_realloc(void *ptr, size_t size);
extern void   stbi__callback_free(void *ptr);

#define STBI_MALLOC(sz)           stbi__callback_malloc(sz)
#define STBI_REALLOC(p,newsz)     stbi__callback_realloc(p,newsz)
#define STBI_FREE(p)              stbi__callback_free(p)
#define STBIW_MALLOC(sz)          stbi__callback_malloc(sz)
#define STBIW_REALLOC(p,newsz)    stbi__callback_realloc(p,newsz)
#define STBIW_FREE(p)             stbi__callback_free(p)

// Include all required library headers
// #include "sokol_app.h"
// #include "sokol_gfx.h"
// #include "sokol_gl.h"
// #include "sokol_time.h"
// #include "sokol_audio.h"
// #include "sokol_log.h"
// #include "fontstash.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_image_resize2.h"

// Wrapper functions for STB write options
void set_png_compression_level(int level) {
	stbi_write_png_compression_level = level;
}

void write_force_png_filter(int level){
	stbi_write_force_png_filter = level;
}

void write_tga_with_rle(int level) {
	stbi_write_tga_with_rle = level;
}
#endif// gama_impl_included
