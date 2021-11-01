#include "CORE.h"

CORE_OBJECT_DEFINE(TestObject);
extern void TestObject_Create(TestObject *instance_ptr);

void Test_CORE_Object(void)
{
	TestObject 	instance;
	TestObject 	instance_ptr;
	void  		*instance_ptr_as_void;

	TestObject_Create(&instance);

	instance_ptr_as_void = instance;
	CORE_AssertObjectType(instance_ptr_as_void, TestObject);
	instance_ptr = (TestObject) instance_ptr_as_void;
	CORE_Assert(instance_ptr == instance);

	/**
	 * 		EXPECT: `instance_ptr` is NULL assert
	 */
	// void *error_ptr = NULL;
	// CORE_AssertObjectType(error_ptr, TestObject);
	// instance_ptr = (TestObject) error_ptr;

	/**
	 * 		EXPECT: `&uint8_value` is not a `TestObject` assert
	 */
	// uint8 uint8_value = 254;
	// CORE_AssertObjectType(&uint8_value, TestObject);
	// instance_ptr = (TestObject) &uint8_value;
}
