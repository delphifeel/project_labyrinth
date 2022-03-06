extern void Test_CORE_Object(void);
extern void Test_CORE_Array(void);

#include "CORE.h"

void CORETests_Run()
{
	Test_CORE_Object();
	Test_CORE_Array();
}
