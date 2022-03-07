#ifndef _CORE_ARRAY_PRIVATE_H_
#define _CORE_ARRAY_PRIVATE_H_

#include "CORE-debug.h"

#define _CARR_VAR(NAME, SUFFIX) 			(_##NAME_##SUFFIX)

#define _CSafeArray_CheckBounds(NAME, INDEX)                 	\
	CORE_AssertWithMessage(                                 	\
		INDEX < _CARR_VAR(NAME, capacity),                   	\
		"`%s` error: out of bounds\n",        		     		\
		#NAME 													\
	)


#endif