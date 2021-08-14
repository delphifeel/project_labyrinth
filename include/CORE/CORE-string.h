#ifndef __CORE_STRING_H__
#define __CORE_STRING_H__

#include "CORE-types.h"
#include "CORE-object.h"

CORE_OBJECT_INTERFACE(CORE_String,
	char 	*Buffer;
	uint32 	Size;
	uint32	MaxSize;
)

void	CORE_String_Create			(CORE_String*, uint32 CustomSize);
void	CORE_String_Free			(CORE_String*);
void	CORE_String_Concat			(CORE_String, const char *Value);
void	CORE_String_GetBuffer		(CORE_String, char **OUT_Buffer);
void	CORE_String_GetSize			(CORE_String, uint32 *OUT_Size);

#endif
