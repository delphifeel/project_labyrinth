#ifndef __CORE_DEBUG_H__
#define __CORE_DEBUG_H__

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#define CORE_DebugAssert 							printf
#define CORE_DebugPrint 							printf
#define CORE_DebugError(MESSAGE) 					(CORE_DebugPrint("ERROR (%s, line %d) %s\n", __FILE__, __LINE__, MESSAGE))
#define CORE_DebugAbort(MESSAGE) 					(CORE_DebugPrint("ABORT (%s, line %d) %s\n", __FILE__, __LINE__, MESSAGE), abort())

#define CORE_DebugAssertPointer(PTR) 				((PTR) != NULL ? TRUE : CORE_DebugAbort("Pointer is NULL"))
#define CORE_DebugCast(TO_TYPE, VALUE) 				( (TO_TYPE) VALUE )

#endif