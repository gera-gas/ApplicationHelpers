#include <iostream>
#include <sstream>
#include "Logger/Logger.h"

namespace helper {

	void logDebug(const char *msg)
	{
		if (helper::isDebuggerPresent()) {
			helper::outputDebugString(msg);
		}
		else {
			std::cout << msg << std::endl;
		}
	}
	
	void logAssert(const char *filename, const char *func, const int linenum)
	{
		std::string outLine("Assertion failed in file: ");

		outLine += filename;
		outLine += ", function: ";
		outLine += func;
		outLine += ", line: ";
		outLine += std::to_string(linenum);

		logDebug(outLine.c_str());
	}

} /* helper */
