#ifndef SYSTEM_PLATFORM_DEFINES_H
#define SYSTEM_PLATFORM_DEFINES_H

#if (defined (_WIN32) || defined (_WIN64))
# define HELPER_PLATROM_WINDOWS 1
#elif defined (LINUX) || defined (__linux__) || defined (__linux)
# define HELPER_PLATROM_LINUX 1
#elif defined (__APPLE_CPP__) || defined(__APPLE_CC__) || defined(__APPLE__)
# define HELPER_PLATROM_APPLE 1
#elif defined (__FreeBSD__)
# define HELPER_PLATROM_FREEBSD 1
#else
# error "Unknown target platform."
#endif

#if defined (_MSC_VER)
# define DEBUG_BREAK { __debugbreak(); }
#elif defined (__i386__) || defined (__x86_64__)
# define DEBUG_BREAK { __asm__ volatile("int $0x03"); }
#else
# define DEBUG_BREAK { __builtin_trap(); }
#endif

#if defined (DEBUG) || defined (_DEBUG) || ! (defined (NDEBUG) || defined (_NDEBUG))
# define HELPER_PLATROM_DEBUG 1
#endif

namespace helper {

	bool isDebuggerPresent(void);
	void outputDebugString(const char *);

} /* helper */


#endif /* SYSTEM_PLATFORM_DEFINES_H */
