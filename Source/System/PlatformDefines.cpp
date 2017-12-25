#include <iostream>
#include "System/PlatformDefines.h"

#if defined (HELPER_PLATROM_WINDOWS)
# include "windows.h"
#elif defined (HELPER_PLATROM_LINUX)
# include <sys/stat.h>
# include <string.h>
# include <fcntl.h>
# include <fstream>
#elif defined (HELPER_PLATROM_APPLE)
#endif

namespace helper {
	
	bool isDebuggerPresent(void)
	{
#if defined (HELPER_PLATROM_WINDOWS)
		 return IsDebuggerPresent();

#elif defined (HELPER_PLATROM_LINUX)
		 std::ifstream ifs("/proc/self/status");

		 if (!ifs) {
			 return false;
		 }

		 std::string tracerPidId("TracerPid:");

		 for( std::string line; std::getline( ifs, line ); )
		 {
			 if(line.length() > 0) {
				 char *cline = (char*)line.c_str();
				 char *tracerPid = strstr(cline, tracerPidId.c_str());

				 if (tracerPid) {
					 return !!atoi(tracerPid + tracerPidId.length()+1 /*skip \t */);
				 }
			 }
		 }

		 return false;

#elif defined (HELPER_PLATROM_APPLE)
		int                 junk;
		int                 mib[4];
		struct kinfo_proc   info;
		size_t              size;

		// Initialize the flags so that, if sysctl fails for some bizarre 
		// reason, we get a predictable result.

		info.kp_proc.p_flag = 0;

		// Initialize mib, which tells sysctl the info we want, in this case
		// we're looking for information about a specific process ID.

		mib[0] = CTL_KERN;
		mib[1] = KERN_PROC;
		mib[2] = KERN_PROC_PID;
		mib[3] = getpid();

		// Call sysctl.

		size = sizeof(info);
		junk = sysctl(mib, sizeof(mib) / sizeof(*mib), &info, &size, NULL, 0);
		assert(junk == 0);

		// We're being debugged if the P_TRACED flag is set.

		return ((info.kp_proc.p_flag & P_TRACED) != 0);
#endif
	}


	void outputDebugString(const char *msg)
	{
#if defined (HELPER_PLATROM_WINDOWS)
		std::string s(msg);
		s += "\n\r";
		OutputDebugString(s.c_str());

#elif defined (HELPER_PLATROM_LINUX)
		std::cerr << msg << std::endl;

#elif defined (HELPER_PLATROM_APPLE)
		fputs(msg, stderr);
		fputs("\n", stderr);
		fflush(stderr);
#endif
	}

} /* helper */
