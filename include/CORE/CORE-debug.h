#ifndef __CORE_DEBUG_H__
#define __CORE_DEBUG_H__

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#define CORE_DebugAssert 							printf
#define CORE_DebugPrint 							printf
#define CORE_DebugError 							printf
#define CORE_DebugAbort(MESSAGE, FILE, LINE) 		(CORE_DebugError("ABORT (%s, line %d) %s\n", FILE, LINE, MESSAGE), abort())

#define CORE_DebugAssertPointer(PTR) 				((PTR) != NULL ? TRUE : CORE_DebugAbort("Pointer is NULL", __FILE__, __LINE__))
#define CORE_DebugCast(TO_TYPE, VALUE) 				( (TO_TYPE) VALUE )

#endif
