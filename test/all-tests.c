#include "modules/modules-tests.h"
#include "libs/libs-tests.h"

// TODO: make tests as libraries (CMake add_library)
int main(void)
{
	CORE_DebugStdOut("[TESTS STARTED]\n");

	Modules_TestsRun();
	Libs_TestsRun();

	CORE_DebugStdOut("[ALL TESTS PASSED]\n");
	return 0;
}