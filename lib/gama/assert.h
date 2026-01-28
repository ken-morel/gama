#ifndef GAMA_ASSERT_H_INCLUDED
#define GAMA_ASSERT_H_INCLUDED

#ifdef __ZIG_CC__
#include <assert.h>
#define gm_static_assert static_assert
#else
#define gm_static_assert(expr, msg)
#endif

#endif // max_requests_per_second
