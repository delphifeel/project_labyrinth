#ifndef __CORE_STRING_H__
#define __CORE_STRINg_H__

#include <stdio.h>
#include <string.h>


#define CORE_StringCopy 		strcopy
#define CORE_StringCopyLength 	strncopy
#define CORE_StringCat 			strcat
#define CORE_StringCatLength 	strncat
#define CORE_StringLength 		strlen
#define CORE_StringPrint 		sprintf

#define CORE_String(SIZE) 		char[SIZE ? SIZE : ]



#endif
