#ifndef _CORE_DEBUG_H_
#define _CORE_DEBUG_H_

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "CORE-debug-private.h"


/**
 *  ___________________________________________________________________________________________________________
 * |																										   |																									   |
 * |												CONSTANTS 												   |
 * |___________________________________________________________________________________________________________| 
 * 
 */
// TODO(delphifeel): move to build process (-D flag)
#define CORE_DEBUG_ENABLED

#ifdef CORE_SET_TESTING_ENABLED
	#define CORE_TESTING_ENABLED
#endif

#define CORE_DebugPrint 								printf

#define _CORE_DEBUG_MESSAGE_PRE(_TYPE)					(CORE_DebugPrint("[%s] (%s:%d) %s() ", (_TYPE), __FILE__, __LINE__, __func__))


/**
 *  ___________________________________________________________________________________________________________
 * |																										   |																									   |
 * |												ASSERTS 												   |
 * |___________________________________________________________________________________________________________| 
 * 
 */
#define CORE_Assert(EXPRESSION) 						(	(EXPRESSION) ? (void) TRUE : (_CORE_DEBUG_MESSAGE_PRE("ASSERT FAILED"), CORE_DebugPrint("%s\n", #EXPRESSION), exit(0))	)
#define CORE_AssertWithMessage(EXPRESSION, ...) 		(	(EXPRESSION) ? (void) TRUE : (_CORE_DEBUG_MESSAGE_PRE("ASSERT FAILED"), CORE_DebugPrint(__VA_ARGS__), exit(0))	)
#define CORE_AssertPointer(PTR) 						(	CORE_AssertWithMessage((PTR) != NULL, "`%s` is NULL\n", #PTR)	)
#define CORE_Abort(...) 								(	_CORE_DEBUG_MESSAGE_PRE("ABORT"), CORE_DebugPrint(__VA_ARGS__), abort()	)

/**
 *  ___________________________________________________________________________________________________________
 * |																										   |																									   |
 * |											DEBUG LEVEL 												   |
 * |___________________________________________________________________________________________________________| 
 * 
 */

#define CORE_DEBUG_LEVEL_NONE		(0)
#define CORE_DEBUG_LEVEL_ERROR		(1)
#define CORE_DEBUG_LEVEL_WARNING	(2)
#define CORE_DEBUG_LEVEL_INFO		(3)

#define CORE_DEFAULT_DEBUG_LEVEL 		(CORE_DEBUG_LEVEL_INFO)

#if defined(CORE_TESTING_ENABLED)
	#define CORE_DEBUG_LEVEL 			(CORE_DEBUG_LEVEL_NONE)
#elif !defined(CORE_TESTING_ENABLED) && defined(CORE_SET_DEBUG_LEVEL)
	#define CORE_DEBUG_LEVEL 			(CORE_SET_DEBUG_LEVEL) 			
#else
	#define CORE_DEBUG_LEVEL 			(CORE_DEFAULT_DEBUG_LEVEL)
#endif

#if CORE_DEBUG_LEVEL >= CORE_DEBUG_LEVEL_INFO
	#define CORE_DebugInfo(...) 	(	_CORE_DEBUG_MESSAGE_PRE("INFO"), 		CORE_DebugPrint(__VA_ARGS__)	)
#else
	#define CORE_DebugInfo(...) 	((void) 0)
#endif

#if CORE_DEBUG_LEVEL >= CORE_DEBUG_LEVEL_WARNING
	#define CORE_DebugWarning(...) 	(	_CORE_DEBUG_MESSAGE_PRE("WARNING"), 	CORE_DebugPrint(__VA_ARGS__)	)
#else
	#define CORE_DebugWarning(...) 	((void) 0)
#endif

#if CORE_DEBUG_LEVEL >= CORE_DEBUG_LEVEL_ERROR
	#define CORE_DebugError(...) 	(	_CORE_DEBUG_MESSAGE_PRE("ERROR"), 		CORE_DebugPrint(__VA_ARGS__)	)
#else
	#define CORE_DebugError(...) 	((void) 0)
#endif


#endif
