#include "CORE.h"

CORE_OBJECT_DEFINE(TestObject);

CORE_OBJECT_INTERFACE(TestObject,
	uint32 	field_1;
	uint32 	field_2;
);

void TestObject_Create(TestObject *instance_ptr)
{
	CORE_OBJECT_CREATE(instance_ptr, TestObject);
}

