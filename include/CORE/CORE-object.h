#ifndef __CORE_OBJECT_H__
#define __CORE_OBJECT_H__

#include "CORE-memory.h"

#define CORE_OBJECT_DEFINE(TYPE_NAME) 			\
typedef struct TYPE_NAME##_STRUCT *TYPE_NAME 	\

#define CORE_OBJECT_INTERFACE(TYPE_NAME, TYPE_STRUCT_INTERFACE) 	\
struct TYPE_NAME##_STRUCT 											\
{																	\
	TYPE_STRUCT_INTERFACE											\
}																	\

#define CORE_OBJECT_CREATE(OBJECT_PTR, TYPE) 		( *OBJECT_PTR = CORE_MemAlloc(sizeof(struct TYPE##_STRUCT)) )
#define CORE_OBJECT_FREE(OBJECT_PTR) 				( CORE_MemFree(*OBJECT_PTR), *OBJECT_PTR = NULL )

#endif
