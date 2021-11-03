#ifndef _CORE_DEBUG_H_
#define _CORE_DEBUG_H_

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "CORE-debug-private.h"


/**
 *  ___________________________________________________________________________________________________________
 * |																										   |																									   
 * |												CONSTANTS 												   |
 * |___________________________________________________________________________________________________________| 
 * 
 */
// TODO(delphifeel): move to build process (-D flag)
#define CORE_DEBUG_ENABLED

#ifdef CORE_SET_TESTING_ENABLED
	#define CORE_TESTING_ENABLED
#endif

#ifndef CORE_SET_DEBUG_COLORED
	#define _COREDEBUG_FUNC_SYMBOL			("")
	#define _COREDEBUG_INFO_SYMBOL			("")
	#define _COREDEBUG_RESET_SYMBOL			("")
	#define _COREDEBUG_ERROR_SYMBOL			("")
	#define _COREDEBUG_WARN_SYMBOL			("")
	#define _COREDEBUG_FILE_SYMBOL			("")
#else
	#define _COREDEBUG_COLOR_SYMBOL(_COLOR)		("\033[1;" #_COLOR "m")
	#define _COREDEBUG_RESET_SYMBOL				("\033[0m")
	#define _COREDEBUG_ERROR_SYMBOL				(_COREDEBUG_COLOR_SYMBOL(31))
	#define _COREDEBUG_WARN_SYMBOL				(_COREDEBUG_COLOR_SYMBOL(35))
	#define _COREDEBUG_INFO_SYMBOL				(_COREDEBUG_COLOR_SYMBOL(36))
	#define _COREDEBUG_FUNC_SYMBOL				(_COREDEBUG_COLOR_SYMBOL(34))
	#define _COREDEBUG_FILE_SYMBOL				(_COREDEBUG_COLOR_SYMBOL(04))
#endif

#define CORE_DebugPrint 	printf


static const char *_CORE_module_name = NULL;
static inline const char *_CORE_GetModuleName(const char *file_name)
{
	const char *temp_ptr;


	if (_CORE_module_name == NULL)
	{
		temp_ptr = file_name;
		while (*temp_ptr != 0)
		{
			if ((*temp_ptr == '/') ||
				(*temp_ptr == '\\'))
			{
				_CORE_module_name = temp_ptr;
			}

			temp_ptr++;
		}

		if (_CORE_module_name == NULL)
		{
			_CORE_module_name = file_name;
		}
		else
		{
			_CORE_module_name++;
		}
	}

	return _CORE_module_name;
}


#define _CORE_DEBUG_MESSAGE_PRE(_TYPE, _TYPE_SYMBOL)													\
		(CORE_DebugPrint(																				\
			"%s[%s]%s %s(%s:%d)%s %s%s()%s ", 															\
			(_TYPE_SYMBOL), 																			\
			(_TYPE), 																					\
			_COREDEBUG_RESET_SYMBOL,																	\
			_COREDEBUG_FILE_SYMBOL,																		\
			_CORE_GetModuleName(__FILE__),																\
			__LINE__, 																					\
			_COREDEBUG_RESET_SYMBOL,																	\
			_COREDEBUG_FUNC_SYMBOL,																		\
			__func__,																					\
			_COREDEBUG_RESET_SYMBOL																		\
		))



/**
 *  ___________________________________________________________________________________________________________
 * |																										   |																									   
 * |												ASSERTS 												   |
 * |___________________________________________________________________________________________________________| 
 * 
 */
#define CORE_Assert(EXPRESSION) 						(	(EXPRESSION) ? (void) TRUE : (_CORE_DEBUG_MESSAGE_PRE("ASSERT FAILED", _COREDEBUG_ERROR_SYMBOL), CORE_DebugPrint("%s\n", #EXPRESSION), exit(0))	)
#define CORE_AssertWithMessage(EXPRESSION, ...) 		(	(EXPRESSION) ? (void) TRUE : (_CORE_DEBUG_MESSAGE_PRE("ASSERT FAILED", _COREDEBUG_ERROR_SYMBOL), CORE_DebugPrint(__VA_ARGS__), exit(0))	)
#define CORE_AssertPointer(PTR) 						(	CORE_AssertWithMessage((PTR) != NULL, "`%s` is NULL\n", #PTR)	)
#define CORE_Abort(...) 								(	_CORE_DEBUG_MESSAGE_PRE("ABORT", _COREDEBUG_ERROR_SYMBOL), CORE_DebugPrint(__VA_ARGS__), abort()	)

/**
 *  ___________________________________________________________________________________________________________
 * |																										   |																									   
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
	#define CORE_DebugInfo(...) 	(	_CORE_DEBUG_MESSAGE_PRE("INFO", _COREDEBUG_INFO_SYMBOL), 		CORE_DebugPrint(__VA_ARGS__)	)
#else
	#define CORE_DebugInfo(...) 	((void) 0)
#endif

#if CORE_DEBUG_LEVEL >= CORE_DEBUG_LEVEL_WARNING
	#define CORE_DebugWarning(...) 	(	_CORE_DEBUG_MESSAGE_PRE("WARNING", _COREDEBUG_WARN_SYMBOL), 	CORE_DebugPrint(__VA_ARGS__)	)
#else
	#define CORE_DebugWarning(...) 	((void) 0)
#endif

#if CORE_DEBUG_LEVEL >= CORE_DEBUG_LEVEL_ERROR
	#define CORE_DebugError(...) 	(	_CORE_DEBUG_MESSAGE_PRE("ERROR", _COREDEBUG_ERROR_SYMBOL), 		CORE_DebugPrint(__VA_ARGS__)	)
#else
	#define CORE_DebugError(...) 	((void) 0)
#endif


#endif
