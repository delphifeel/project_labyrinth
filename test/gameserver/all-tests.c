extern void Test_CommandsListener();
extern void Test_DisjointSet();
extern void Test_LabGeneration();
extern void Test_LabSession();
extern void Test_Command();
extern void Test_CommandsProcessor();

#include "CORE.h"

int main()
{
	Test_DisjointSet();
	Test_LabGeneration();
	Test_LabSession();	
	Test_Command();
	Test_CommandsProcessor();
}
