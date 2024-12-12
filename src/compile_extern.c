/*
 * File for compiling used c library files and headers
 * Since these will throw alot of compile errors beacuse of incompatible
 * conversions
 */

#include <emmintrin.h> // For SSE2 intrinsics

#define OGG_IMPL
#define VORBIS_IMPL

#include "audio/extern/minivorbis.h"
