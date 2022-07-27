#include "iosystem/iosystem-tests.h"
#include "labyrinth/labyrinth-tests.h"

#define RUN_TEST(NAME) do {                 \
    CORE_DebugStdOut(#NAME " tests...");    \
    NAME##_TestsRun();                      \
    CORE_DebugStdOut(" passed\n");          \
} while (0)

void Modules_TestsRun(void)
{
    RUN_TEST(IOSystem);
    RUN_TEST(Labyrinth);
}