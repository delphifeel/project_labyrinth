#ifndef _CORE_DEBUG_H_
#define _CORE_DEBUG_H_

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

// TODO(delphifeel): move to build process (-D flag)
#define CORE_DEBUG_ENABLED

#include "CORE-debug-private.h"

#define CORE_DebugPrint 								printf
#define CORE_Assert(EXPRESSION) 						(	(EXPRESSION) ? TRUE : (CORE_DebugPrint("ASSERT FAILED (%s, %s, line %d)\t", __FILE__, __func__, __LINE__), CORE_DebugPrint("%s\n", #EXPRESSION))	)
#define CORE_AssertWithMessage(EXPRESSION, ...) 		(	(EXPRESSION) ? TRUE : (CORE_DebugPrint("ASSERT FAILED (%s, %s, line %d)\t", __FILE__, __func__, __LINE__), CORE_DebugPrint(__VA_ARGS__))	)
#define CORE_DebugError(...) 							(	CORE_DebugPrint("ERROR (%s, %s, line %d)\t", __FILE__, __func__, __LINE__), CORE_DebugPrint(__VA_ARGS__)	)
#define CORE_DebugAbort(...) 							(	CORE_DebugPrint("ABORT (%s, %s, line %d)\t", __FILE__, __func__, __LINE__), CORE_DebugPrint(__VA_ARGS__), abort()	)
#define CORE_AssertPointer(PTR) 						(	CORE_AssertWithMessage((PTR) != NULL, "Pointer is NULL\n")	)

#endif
