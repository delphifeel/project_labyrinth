#include <assert.h>
#include <string.h>
#include "Core_String.h"

void Test_StringInit(void)
{
	CoreString StringInst;


	assert(StringInst.buffer != NULL);
	assert(StringInst.size == 0);
	assert(StringInst.max_size == 256);
}

void Test_StrintInitWithCustomSize(void)
{
	int CustomSize = 20;
	CoreString StringInst(CustomSize);



	assert(StringInst.buffer != NULL);
	assert(StringInst.size == 0);
	assert(StringInst.max_size == CustomSize);
}

void Test_StringConcat(void)
{
	CoreString StringInst;
	int CustomSize;
	char *CustomText = "Hello, world";


	CustomSize = 20;

	StringInst.Concat(CustomText);
	assert(strcmp(StringInst.buffer, CustomText) == 0);
}

void Test_StringGetSize(void)
{
	CoreString StringInst;
	int CustomSize, Size;
	char *CustomText = "Hello, world";


	CustomSize = 20;

	StringInst.Concat(CustomText);

	Size = StringInst.GetSize();
	assert(Size == strlen(CustomText));
}

void Test_StringGetBuffer(void)
{
	CoreString StringInst;
	int CustomSize;
	char *BufferPtr;


	CustomSize = 20;

	BufferPtr = StringInst.GetBuffer();
	assert(BufferPtr == StringInst.buffer);
}

void Test_StringBecomeBigger(void)
{
	CoreString StringInst;
	int CustomSize;
	char *CustomText = "Hello, world";


	CustomSize = 5;

	StringInst.Concat(CustomText);
	assert(StringInst.max_size > CustomSize);
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
