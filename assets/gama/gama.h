
#ifndef GAMA_INCLUDED
#define GAMA_INCLUDED

#include "color.h"

#include "app.h"
#include "shape.h"

#include <stdio.h>

#ifdef BACKEND_GLFW
#include "backends/glfw.h"
#endif // BACKEND_GLFW
#ifndef BACKEND_GLFW
#ifdef BACKEND_WIN32
#include "backends/win32.h"
#endif
#ifndef BACKEND_WIN32
#error "Gama cannot run, no backend specified"
#endif // BACKEND_WIN32
#endif

#endif // GAMA_INCLUDED
