#include <stddef.h>
#include <string.h>
#include "../../include/CORE/CORE_String.h"
#include "../../include/CORE/CORE-memory.h"
#include "../../include/CORE/CORE_Object.h"

#define DEFAULT_BUFFER_SIZE (256)

void CORE_String_Create(CORE_String *InstancePtr, uint32 CustomSize) {
	uint32 Size;


	CORE_OBJECT_CREATE(InstancePtr, CORE_String);
	
	Size = CustomSize > 0 ? CustomSize : DEFAULT_BUFFER_SIZE;

	(*InstancePtr)->Buffer = CORE_MemAlloc(Size * sizeof(char));
	(*InstancePtr)->Size = 0;
	(*InstancePtr)->MaxSize = Size;
}

void CORE_String_Free(CORE_String *InstancePtr) {
	CORE_MemFree((*InstancePtr)->Buffer);

	CORE_OBJECT_FREE(InstancePtr);
}

void CORE_String_Concat(CORE_String Instance, const char *Value) {
	const char *Iter = Value;
	while (1) {
		if (Instance->Size == Instance->MaxSize) {
			Instance->MaxSize *= 2;
			Instance->Buffer = CORE_MemRealloc(Instance->Buffer, Instance->MaxSize);
		}
		
		Instance->Buffer[Instance->Size] = *Iter;
		
		if (*Iter == '\0') {
			break;
		}

		Instance->Size++;
		Iter++;
	}
}

void CORE_String_GetBuffer(CORE_String Instance, char **OUT_Buffer) {
	*OUT_Buffer = Instance->Buffer;
}

void CORE_String_GetSize(CORE_String Instance, uint32 *OUT_Size) {
	*OUT_Size = Instance->Size;
}
