#include "CORE.h"

#define CHAR_ARRAY_SIZE 	(10)

static void TEST_ArrayAsParams(char 	array[], 
							   uint32 	array_max_size, 
							   uint32 	array_size)
{
	CSafeArray_From(array, array_max_size, array_size);


	CORE_Assert(CSafeArray_Capacity(array) == array_max_size);
	CORE_Assert(CSafeArray_Size(array) == array_size);
}

static void TEST_1(void)
{
	CSafeArray_Create(char_array, char, CHAR_ARRAY_SIZE);
	char char_array_copy[CHAR_ARRAY_SIZE];


	CORE_MemZero(char_array_copy, sizeof(char_array_copy));
	CSafeArray_Add(char_array, 'H');
	CSafeArray_Add(char_array, 'e');
	CSafeArray_Add(char_array, 'l');
	CSafeArray_Add(char_array, 'l');
	CSafeArray_Add(char_array, 'o');
	CSafeArray_Add(char_array, '\0');
	for (uint32 i = 0; i < CSafeArray_Size(char_array); i++)
	{
		char_array_copy[i] = CSafeArray_Get(char_array, i);
	}
	CORE_Assert(CORE_StringEqual(char_array, char_array_copy));

	TEST_ArrayAsParams(char_array, 
					   CSafeArray_Capacity(char_array), 
					   CSafeArray_Size(char_array));
}

static void Test_Integer(void)
{
	CSafeArray_Create(int_array, int, 4);


	CSafeArray_Add(int_array, 1);
	CSafeArray_Add(int_array, 2);
	CSafeArray_Add(int_array, 3);
	CSafeArray_Add(int_array, 4);

	for (int i = 0; i < 4; i++)
	{
		CORE_Assert(i + 1 == CSafeArray_Get(int_array, i));
	}
}

static void Test_FromAllocedArray(void)
{
	struct Man
	{
		uint32 		age;
		const char 	*name;
	};


	struct Man *man_list;
	struct Man bob = {22, "Bob"};
	struct Man sammy = {40, "Sammy"};


	man_list = CORE_MemAlloc(2 * sizeof(struct Man));
	CSafeArray_From(man_list, 2, 0);
	CSafeArray_Add(man_list, bob);
	CSafeArray_Add(man_list, sammy);
	CORE_Assert(
		(CSafeArray_Get(man_list, 0).age == bob.age) &&
		(CSafeArray_Get(man_list, 0).name == bob.name) 
	);
	CORE_Assert(
		(CSafeArray_Get(man_list, 1).age == sammy.age) &&
		(CSafeArray_Get(man_list, 1).name == sammy.name) 
	);

	CORE_Assert(&man_list[0] == CSafeArray_GetPtr(man_list, 0));

	CORE_MemFree(man_list);
}

void Test_CORE_Array(void)
{
	TEST_1();
	Test_Integer();
	Test_FromAllocedArray();
}