#include "CCORE.h"

extern void AuthServerTests_Run(void);
extern void GameServerTests_Run(void);

int main(void)
{
	#if 0
	CORE_DebugPrint("[TESTS] Run AuthServer tests\n");
	AuthServerTests_Run();

	CORE_DebugPrint("[TESTS] Run GameServer tests\n");
	GameServerTests_Run();
	#endif

	return 0;
}