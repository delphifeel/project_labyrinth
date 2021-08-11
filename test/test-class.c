#include "../include/CORE.h"

typedef struct DataStruct
{
	CORE_Handle 	CORE_ClassHandle;

	uint32 			Age;
	CORE_Bool 		IsMan; 			
} DataStruct;

CORE_ClassMakeConstructor(Data);
CORE_ClassMakeDestructor(Data);

typedef struct FakeStruct
{
	CORE_Handle CORE_ClassHandle;

	CORE_Bool   Nothing;
} FakeStruct;

CORE_ClassMakeConstructor(Fake);
CORE_ClassMakeDestructor(Fake);

CORE_Bool Data_SetAge(CORE_Handle DataHandle, uint32 Age)
{
	DataStruct *Data = CORE_ClassCast(DataHandle, DataStruct);

	Data->Age = Age;

	return TRUE;
}

CORE_Bool Data_GetAge(CORE_Handle DataHandle, uint32 *Age)
{
	DataStruct *Data = CORE_ClassCast(DataHandle, DataStruct);

	*Age = Data->Age;

	return TRUE;
}

static void CreateBOB(CORE_Handle *BobDataHandle)
{
	Data_Create(BobDataHandle);
	Data_SetAge(*BobDataHandle, 999);
}

int main()
{
	CORE_Handle BobData;
	CORE_Handle FakeHandle;
	uint32 		Age;


	Fake_Create(&FakeHandle);

	CreateBOB(&BobData);
	Data_GetAge(BobData, &Age);

	CORE_DebugPrint("Age: %d\n", Age);
}