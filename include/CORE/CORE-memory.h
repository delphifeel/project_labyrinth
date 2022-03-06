#ifndef _CORE_MEMORY_H_
#define _CORE_MEMORY_H_

#include <stdlib.h>
#include <string.h>

#define CORE_MemAlloc				malloc
#define CORE_MemCalloc				calloc
#define CORE_MemRealloc 			realloc
#define CORE_MemFree 				free
#define CORE_MemSet 				memset
#define CORE_MemZero(PTR, SIZE)		CORE_MemSet(PTR, 0, SIZE)

#define CORE_StringEqual(_STR1, _STR2)		(strcmp(_STR1, _STR2) == 0 ? TRUE : FALSE)
#define CORE_StringLength(_STR)             (strlen(_STR))

#endif
