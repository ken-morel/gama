#ifndef GAMA_ASSERT_H_INCLUDED
#define GAMA_ASSERT_H_INCLUDED

#ifdef GM_ZCC
#include <assert.h>
#define gm_static_assert static_assert
#else
#define gm_static_assert(expr, msg)
#endif

#endif // max_requests_per_second
