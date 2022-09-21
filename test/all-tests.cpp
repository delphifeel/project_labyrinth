#define RUN_TEST(NAME) do {                 \
    CORE_DebugStdOut(#NAME " tests...\n");  \
    NAME##_TestsRun();                      \
    CORE_DebugStdOut("  PASSED\n");       \
} while (0)

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