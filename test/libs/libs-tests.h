#include "packet/packet-tests.h"

#define RUN_TEST(NAME) do {                 \
    CORE_DebugStdOut(#NAME " tests...");    \
    NAME##_TestsRun();                      \
    CORE_DebugStdOut(" passed\n");          \
} while (0)

void Libs_TestsRun(void)
{
    RUN_TEST(Packet);
}