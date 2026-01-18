#pragma once

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_SIMD

// NOTE: The order is important, major, minor, patch
#define GAMA_VERSION_MAJOR 0
#define GAMA_VERSION_MINOR 1
#define GAMA_VERSION_PATCH 1

#ifndef GM_NO_GAPI
#include "gama/gama.h"
#include "gama/sprite.h"
#endif

#include "gama/3d.h"
#include "gama/animate.h"
#include "gama/key.h"
#include "gama/physics.h"
#include "gama/system.h"
