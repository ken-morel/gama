
#ifndef GAMA_INCLUDED
#define GAMA_INCLUDED

#include "color.h"

#include "app.h"
#include "shape.h"
#include "image.h"

#include <stdio.h>

///////////////////////
#ifndef BACKEND_GLFW
#ifndef BACKEND_WIN32

#ifdef __linux__
#define BACKEND_GLFW
#endif
#ifdef __windows__
#define BACKEND_GLFW
#endif

#endif
#endif
////////////////////////

#ifdef BACKEND_GLFW
#include "backends/glfw.h"

#endif // BACKEND_GLFW

#ifdef BACKEND_WIN32
#include "backends/win32.h"
#endif // Backend win32

#endif // GAMA_INCLUDED
