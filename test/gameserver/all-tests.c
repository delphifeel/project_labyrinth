extern void Test_CommandsListener();
extern void Test_DisjointSet();
extern void Test_LabGeneration();
extern void Test_LabSession();

#include "CORE.h"

int main()
{
	Test_DisjointSet();
	Test_LabGeneration();
	Test_LabSession();	
}
