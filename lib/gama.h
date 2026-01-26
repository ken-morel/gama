#pragma once

#ifndef STBI_NO_SIMD
#define STBI_NO_SIMD
#endif // STBI_NO_SIMD

// NOTE: The order is important, major, minor, patch
#define GAMA_VERSION_MAJOR 0
#define GAMA_VERSION_MINOR 1
#define GAMA_VERSION_PATCH 1

#include "gama/position.h"

#ifndef GM_NO_GAPI
#include "gama/gama.h"
#include "gama/sprite.h"
#endif

#include "gama/animate.h"
#include "gama/key.h"
#include "gama/physics.h"
#include "gama/system.h"

#ifdef __ZIG_CC__
#include <assert.h>
#define gm_static_assert static_assert
#else
#define gm_static_assert(expr, msg)
#endif
