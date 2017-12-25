#include <iostream>
#include "demo_system.h"
#include "LeakDetector/LeakDetector.h"

class ExampleClass {
public:
	ExampleClass() : ADD_TO_LEAKED_OBJECTS_LIST
	{ }

	~ExampleClass()
	{
		REMOVE_FROM_LEAKED_OBJECTS_LIST;
	}

private:
	LEAK_DETECTOR(ExampleClass);
};

void test_leaked()
{
	ExampleClass normalObject;
	ExampleClass *leakedObject = new ExampleClass();

	ADD_TO_SUSPICIOUS_OBJECTS_LIST(&normalObject);
	ADD_TO_SUSPICIOUS_OBJECTS_LIST(leakedObject);
}
