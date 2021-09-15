#ifndef __CORE_DEBUG_H__
#define __CORE_DEBUG_H__

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

// TODO: move to build process (-D flag)
#define CORE_DEBUG_ENABLED


#define CORE_DebugAssert 							printf
#define CORE_DebugPrint 							printf
#define CORE_DebugError(...) 						(	CORE_DebugPrint("ERROR (%s, %s, line %d)\t", __FILE__, __func__, __LINE__), CORE_DebugPrint(__VA_ARGS__)	)
#define CORE_DebugAbort(...) 						(	CORE_DebugPrint("ABORT (%s, %s, line %d)\t", __FILE__, __func__, __LINE__), CORE_DebugPrint(__VA_ARGS__), abort()	)

#define CORE_DebugAssertPointer(PTR) 				((PTR) != NULL ? TRUE : CORE_DebugAbort("Pointer is NULL"))
#define CORE_DebugCast(TO_TYPE, VALUE) 				( (TO_TYPE) VALUE )

#endif
