#ifndef _CORE_DEBUG_H_
#define _CORE_DEBUG_H_

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>


/**
 *  ___________________________________________________________________________________________________________
 * |                                                                                                           |                                                                                                       
 * |                                                CONSTANTS                                                  |
 * |___________________________________________________________________________________________________________| 
 * 
 */

#ifdef CORE_SET_TESTING_ENABLED
    #define CORE_TESTING_ENABLED
#endif

#ifndef CORE_SET_DEBUG_COLORED
    #define _COREDEBUG_FUNC_SYMBOL          ("")
    #define _COREDEBUG_INFO_SYMBOL          ("")
    #define _COREDEBUG_RESET_SYMBOL         ("")
    #define _COREDEBUG_ERROR_SYMBOL         ("")
    #define _COREDEBUG_WARN_SYMBOL          ("")
    #define _COREDEBUG_FILE_SYMBOL          ("")
#else
    #define _COREDEBUG_COLOR_SYMBOL(_COLOR)     ("\033[1;" #_COLOR "m")
    #define _COREDEBUG_RESET_SYMBOL             ("\033[0m")
    #define _COREDEBUG_ERROR_SYMBOL             (_COREDEBUG_COLOR_SYMBOL(31))
    #define _COREDEBUG_WARN_SYMBOL              (_COREDEBUG_COLOR_SYMBOL(35))
    #define _COREDEBUG_INFO_SYMBOL              (_COREDEBUG_COLOR_SYMBOL(36))
    #define _COREDEBUG_FUNC_SYMBOL              (_COREDEBUG_COLOR_SYMBOL(34))
    #define _COREDEBUG_FILE_SYMBOL              (_COREDEBUG_COLOR_SYMBOL(04))
#endif

#define CORE_DebugStdOut(...)   fprintf(stdout, __VA_ARGS__)
#define CORE_DebugStdErr(...)   fprintf(stderr, __VA_ARGS__)


#define _CORE_DEBUG_MESSAGE_PRE(_TYPE, _TYPE_SYMBOL, _FILE, _LINE)                      \
                                                                                        \
            "%s[%s]%s %s(%s:%d)%s %s%s()%s ",                                           \
            (_TYPE_SYMBOL),                                                             \
            (_TYPE),                                                                    \
            _COREDEBUG_RESET_SYMBOL,                                                    \
            _COREDEBUG_FILE_SYMBOL,                                                     \
            _FILE,                                                                      \
            _LINE,                                                                      \
            _COREDEBUG_RESET_SYMBOL,                                                    \
            _COREDEBUG_FUNC_SYMBOL,                                                     \
            __func__,                                                                   \
            _COREDEBUG_RESET_SYMBOL                                                     

#define _CORE_DEBUG_PRINT(_TYPE, _TYPE_SYMBOL)          \
    (CORE_DebugStdOut(_CORE_DEBUG_MESSAGE_PRE(_TYPE, _TYPE_SYMBOL, __FILE__, __LINE__)))

#define _CORE_DEBUG_ERROR(_TYPE, _TYPE_SYMBOL)          \
    (CORE_DebugStdErr(_CORE_DEBUG_MESSAGE_PRE(_TYPE, _TYPE_SYMBOL, __FILE__, __LINE__)))

#define _CORE_DEBUG_ERROR_EX(_TYPE, _TYPE_SYMBOL, _FILE, _LINE)             \
    (CORE_DebugStdErr(_CORE_DEBUG_MESSAGE_PRE(_TYPE, _TYPE_SYMBOL, _FILE, _LINE)))


/**
 *  ___________________________________________________________________________________________________________
 * |                                                                                                           |                                                                                                       
 * |                                                ASSERTS                                                    |
 * |___________________________________________________________________________________________________________| 
 * 
 */
#define CORE_Assert(EXPRESSION)                         (   (EXPRESSION) ? (void) true : (_CORE_DEBUG_ERROR("ASSERT FAILED", _COREDEBUG_ERROR_SYMBOL), CORE_DebugStdErr("%s\n", #EXPRESSION), abort())  )
#define CORE_AssertWithMessage(EXPRESSION, ...)         (   (EXPRESSION) ? (void) true : (_CORE_DEBUG_ERROR("ASSERT FAILED", _COREDEBUG_ERROR_SYMBOL), CORE_DebugStdErr(__VA_ARGS__), abort())  )

#define CORE_AssertWithMessageEx(FILE, LINE, EXPRESSION, ...) \
    (   (EXPRESSION) ? (void) true : (_CORE_DEBUG_ERROR_EX("ASSERT FAILED", _COREDEBUG_ERROR_SYMBOL, FILE, LINE), CORE_DebugStdErr(__VA_ARGS__), abort())   )

#define CORE_AssertPointer(PTR)                         (   CORE_AssertWithMessage((PTR) != NULL, "`%s` is NULL\n", #PTR)   )
#define CORE_AssertIntEqual(A, B)                       ( CORE_AssertWithMessage(A == B, #A " == " #B " (%d vs %d)\n", A, B) )

#define CORE_Abort(...)                                 (   _CORE_DEBUG_ERROR("ABORT", _COREDEBUG_ERROR_SYMBOL), CORE_DebugStdErr(__VA_ARGS__), abort() )

/**
 *  ___________________________________________________________________________________________________________
 * |                                                                                                           |                                                                                                       
 * |                                            DEBUG LEVEL                                                    |
 * |___________________________________________________________________________________________________________| 
 * 
 */

#define CORE_DEBUG_LEVEL_NONE       (0)
#define CORE_DEBUG_LEVEL_ERROR      (1)
#define CORE_DEBUG_LEVEL_WARNING    (2)
#define CORE_DEBUG_LEVEL_INFO       (3)

#define CORE_DEFAULT_DEBUG_LEVEL        (CORE_DEBUG_LEVEL_INFO)

#if defined(CORE_TESTING_ENABLED)
    #define CORE_DEBUG_LEVEL            (CORE_DEBUG_LEVEL_NONE)
#elif !defined(CORE_TESTING_ENABLED) && defined(CORE_SET_DEBUG_LEVEL)
    #define CORE_DEBUG_LEVEL            (CORE_SET_DEBUG_LEVEL)          
#else
    #define CORE_DEBUG_LEVEL            (CORE_DEFAULT_DEBUG_LEVEL)
#endif

#if CORE_DEBUG_LEVEL >= CORE_DEBUG_LEVEL_INFO
    #define CORE_DebugInfo(...)     (   _CORE_DEBUG_PRINT("INFO", _COREDEBUG_INFO_SYMBOL),      CORE_DebugStdOut(__VA_ARGS__)   )
#else
    #define CORE_DebugInfo(...)     ((void) 0)
#endif

#if CORE_DEBUG_LEVEL >= CORE_DEBUG_LEVEL_WARNING
    #define CORE_DebugWarning(...)  (   _CORE_DEBUG_PRINT("WARNING", _COREDEBUG_WARN_SYMBOL),   CORE_DebugStdOut(__VA_ARGS__)   )
#else
    #define CORE_DebugWarning(...)  ((void) 0)
#endif

#if CORE_DEBUG_LEVEL >= CORE_DEBUG_LEVEL_ERROR
    #define CORE_DebugError(...)    (   _CORE_DEBUG_ERROR("ERROR", _COREDEBUG_ERROR_SYMBOL),        CORE_DebugStdErr(__VA_ARGS__)   )
#else
    #define CORE_DebugError(...)    ((void) 0)
#endif


#endif