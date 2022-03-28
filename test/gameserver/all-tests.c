extern void Test_CommandsListener();
extern void Test_LabGeneration();
extern void Test_LabSession();
extern void Test_Command();
extern void Test_CommandsProcessor();

#include "CCORE.h"

void GameServerTests_Run()
{
	Test_LabGeneration();
	Test_LabSession();	
	Test_Command();
	Test_CommandsProcessor();
}
