#include "CORE.h"

extern void CORETests_Run(void);
extern void AuthServerTests_Run(void);
extern void GameServerTests_Run(void);

int main()
{
	CORE_DebugPrint("[TESTS] Run CORE tests\n");
	CORETests_Run();

	CORE_DebugPrint("[TESTS] Run AuthServer tests\n");
	AuthServerTests_Run();

	CORE_DebugPrint("[TESTS] Run GameServer tests\n");
	GameServerTests_Run();
}