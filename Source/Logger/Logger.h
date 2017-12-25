#ifndef LOGGER_LOGGER_H
#define LOGGER_LOGGER_H

#include "System/PlatformDefines.h"

#if defined (HELPER_PLATROM_DEBUG)
# define DBGLOG(msg) logDebug(msg)
#else
# define DBGLOG(msg)
#endif

#if defined (HELPER_PLATROM_DEBUG)
# define ASSERT_FALSE logAssert(__FILE__, __FUNCTION__, __LINE__); if (helper::isDebuggerPresent()) DEBUG_BREAK else exit(1);
#else
# define ASSERT_FALSE
#endif

#if defined (HELPER_PLATROM_DEBUG)
# define ASSERT(expression) if (!(expression)) { ASSERT_FALSE; }
#else
# define ASSERT(expression)
#endif

namespace helper {

	void logDebug(const char *msg);
	void logAssert(const char *filename, const char *func, const int linenum);

} /* helper */

#endif /* LOGGER_LOGGER_H */
