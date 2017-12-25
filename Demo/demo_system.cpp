#include <iostream>
#include "demo_system.h"

void test_system()
{
	if (helper::isDebuggerPresent()) {
		helper::outputDebugString("Debug test message...");
		DEBUG_BREAK;
	}
	else {
		std::cout << "Run without debugger..." << std::endl;
	}
}
