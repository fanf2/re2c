#ifndef _RE2C_UTIL_DEBUG_ASSERT_
#define _RE2C_UTIL_DEBUG_ASSERT_

#ifdef RE2C_DEBUG

#include <assert.h>
#define DASSERT(x) assert(x)

#else // RE2C_DEBUG

#define DASSERT(x)

#endif // RE2C_DEBUG

#endif // _RE2C_UTIL_DEBUG_ASSERT_
