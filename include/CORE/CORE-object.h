#ifndef _CORE_OBJECT_H_
#define _CORE_OBJECT_H_

#include "CORE-memory.h"

#define _CORE_OBJECT_NAME_STR_PTR(TYPE_NAME)			(TYPE_NAME##_OBJECT_NAME)
#define _CORE_OBJECT_NAME_STR_PTR_GETTER(TYPE_NAME)		(TYPE_NAME##_OBJECT_NAME_GET)

#define _CORE_OBJECT_FIELDS(_INTERFACE) 							\
	const char *_object_name;										\
	_INTERFACE														\

#define CORE_OBJECT_DEFINE(TYPE_NAME) 											\
	typedef struct TYPE_NAME##_STRUCT *(TYPE_NAME); 								\
	extern const char *_CORE_OBJECT_NAME_STR_PTR_GETTER(TYPE_NAME)(void);		\
	extern const char *TYPE_NAME##_GetObjectName(TYPE_NAME _instance)			\

#define CORE_OBJECT_INTERFACE(TYPE_NAME, TYPE_STRUCT_INTERFACE) 					\
	struct TYPE_NAME##_STRUCT 														\
	{																				\
		_CORE_OBJECT_FIELDS(TYPE_STRUCT_INTERFACE)									\
	};																				\
																					\
	static const char *_CORE_OBJECT_NAME_STR_PTR(TYPE_NAME) = #TYPE_NAME;			\
																					\
	const char *_CORE_OBJECT_NAME_STR_PTR_GETTER(TYPE_NAME)(void)					\
	{																				\
		return _CORE_OBJECT_NAME_STR_PTR(TYPE_NAME);								\
	}																				\
																					\
	const char *TYPE_NAME##_GetObjectName(TYPE_NAME _instance)						\
	{																				\
		return _instance->_object_name;												\
	}																				\
																					\
	typedef TYPE_NAME _DUMMY_##TYPE_NAME											\

#define CORE_OBJECT_CREATE(OBJECT_PTR, TYPE_NAME) 	do { 						\
	CORE_AssertPointer(OBJECT_PTR);												\
	*(OBJECT_PTR) = CORE_MemAlloc(sizeof(struct TYPE_NAME##_STRUCT)); 			\
	(*(OBJECT_PTR))->_object_name = _CORE_OBJECT_NAME_STR_PTR(TYPE_NAME);		\
} while (0)																		\


#define CORE_OBJECT_FREE(OBJECT_PTR) 	do {		\
	CORE_AssertPointer(OBJECT_PTR); 				\
	CORE_MemFree(*(OBJECT_PTR));					\
	*(OBJECT_PTR) = NULL;							\
} while (0)											\

#define CORE_AssertObjectType(OBJECT, TYPE_NAME)	do {											\
	CORE_AssertPointer(OBJECT);																		\
	CORE_AssertWithMessage(																			\
		TYPE_NAME##_GetObjectName(OBJECT) == _CORE_OBJECT_NAME_STR_PTR_GETTER(TYPE_NAME)(), 		\
		"`%s` is not a `%s`\n", #OBJECT, #TYPE_NAME													\
	);																								\
} while (0)																							\

#endif
