extern void Test_CORE_TCPServer(void);
extern void Test_CORE_Object(void);

#include "CORE.h"

void CORETests_Run()
{
	Test_CORE_TCPServer();
	Test_CORE_TCPClient();
	Test_CORE_Object();
}
