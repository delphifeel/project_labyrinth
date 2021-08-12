#include <assert.h>
#include <string.h>
#include "../include/CORE.h"

void Test_StringInit(void)
{
	CORE_String StringInst;


	CORE_String_Create(&StringInst, 0);
	assert(StringInst->Buffer != NULL);
	assert(StringInst->Size == 0);
	assert(StringInst->MaxSize == 256);
	CORE_String_Free(&StringInst);
}

void Test_StrintInitWithCustomSize(void)
{
	CORE_String StringInst;
	uint32 CustomSize;


	CustomSize = 20;

	CORE_String_Create(&StringInst, CustomSize);
	assert(StringInst->Buffer != NULL);
	assert(StringInst->Size == 0);
	assert(StringInst->MaxSize == CustomSize);
	CORE_String_Free(&StringInst);
}

void Test_StringConcat(void)
{
	CORE_String StringInst;
	uint32 CustomSize;
	char *CustomText = "Hello, world";


	CustomSize = 20;

	CORE_String_Create(&StringInst, CustomSize);
	CORE_String_Concat(StringInst, CustomText);
	assert(strcmp(StringInst->Buffer, CustomText) == 0);
	CORE_String_Free(&StringInst);
}

void Test_StringGetSize(void)
{
	CORE_String StringInst;
	uint32 CustomSize, Size;
	char *CustomText = "Hello, world";


	CustomSize = 20;

	CORE_String_Create(&StringInst, CustomSize);
	CORE_String_Concat(StringInst, CustomText);

	CORE_String_GetSize(StringInst, &Size);
	assert(Size == strlen(CustomText));
	CORE_String_Free(&StringInst);
}

void Test_StringGetBuffer(void)
{
	CORE_String StringInst;
	uint32 CustomSize;
	char *BufferPtr;


	CustomSize = 20;

	CORE_String_Create(&StringInst, CustomSize);
	CORE_String_GetBuffer(StringInst, &BufferPtr);
	assert(BufferPtr == StringInst->Buffer);
	CORE_String_Free(&StringInst);
}

void Test_StringBecomeBigger(void)
{
	CORE_String StringInst;
	uint32 CustomSize;
	char *CustomText = "Hello, world";


	CustomSize = 5;

	CORE_String_Create(&StringInst, CustomSize);
	CORE_String_Concat(StringInst, CustomText);
	assert(StringInst->MaxSize > CustomSize);
	CORE_String_Free(&StringInst);
}

int main()
{
	Test_StringInit();
	Test_StrintInitWithCustomSize();
	Test_StringConcat();
	Test_StringGetSize();
	Test_StringGetBuffer();
	Test_StringBecomeBigger();
}
