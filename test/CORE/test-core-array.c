#include "CORE.h"

#define CHAR_ARRAY_SIZE 	(10)

static void TEST_ArrayAsParams(char 	array[], 
							   uint32 	array_max_size, 
							   uint32 	array_size)
{
	CORE_StaticArray_Wrap(array, array_max_size, array_size);


	CORE_Assert(CORE_Array_Capacity(array) == array_max_size);
	CORE_Assert(CORE_Array_Size(array) == array_size);
}

static void TEST_1(void)
{
	CORE_StaticArray(char_array, char, CHAR_ARRAY_SIZE);
	char char_array_copy[CHAR_ARRAY_SIZE];


	CORE_MemZero(char_array_copy, sizeof(char_array_copy));
	CORE_Array_Add(char_array, 'H');
	CORE_Array_Add(char_array, 'e');
	CORE_Array_Add(char_array, 'l');
	CORE_Array_Add(char_array, 'l');
	CORE_Array_Add(char_array, 'o');
	CORE_Array_Add(char_array, '\0');
	for (uint32 i = 0; i < CORE_Array_Size(char_array); i++)
	{
		char_array_copy[i] = CORE_Array_Get(char_array, i);
	}
	CORE_Assert(CORE_StringEqual(char_array, char_array_copy));

	TEST_ArrayAsParams(char_array, 
					   CORE_Array_Capacity(char_array), 
					   CORE_Array_Size(char_array));
}

void Test_CORE_Array(void)
{
	TEST_1();
}