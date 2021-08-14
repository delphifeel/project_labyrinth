#ifndef __CORE_CAST_H__
#define __CORE_CAST_H__

// #include "CORE-debug.h"
// #include "CORE-memory.h"

// #define CORE_ClassHandleDefinition(TYPE) 		(CORE_Handle)(#TYPE ":" __FILE__)
// #define CORE_ClassCheck(HANDLE, TYPE) 			(CORE_DebugAssertPointer(HANDLE), ((TYPE *)(HANDLE))->CORE_ClassHandle == CORE_ClassHandleDefinition(TYPE) ? TRUE : (CORE_DebugPrint("Class missmatch: expect '%s' got '%s'\n", (char *) CORE_ClassHandleDefinition(TYPE), (char *) ((TYPE *)(HANDLE))->CORE_ClassHandle), CORE_DebugAbort("", __FILE__, __LINE__)))
// #define CORE_ClassCast(HANDLE, TYPE) 			(CORE_ClassCheck(HANDLE, TYPE), (TYPE*) HANDLE)

// #define CORE_ClassMakeConstructor(CLASS) 																\
// CORE_Bool CLASS##_Create(CORE_Handle *CLASS##Handle)													\
// {																										\
//     CLASS##Struct *CLASS;																				\
//     CORE_DebugAssertPointer(CLASS##Handle);																\

//     CLASS = CORE_MemAlloc(sizeof(CLASS##Struct));														\
//     (CLASS)->CORE_ClassHandle = CORE_ClassHandleDefinition(CLASS##Struct); 								\
//     CLASS##_InternalCreate(CLASS);                                                                      \
//     *CLASS##Handle = (CORE_Handle) CLASS;																\
//     return TRUE;																						\
// }																										\

// #define CORE_ClassMakeDestructor(CLASS) 																\
// CORE_Bool CLASS##_Delete(CORE_Handle *CLASS##Handle)													\
// {																										\
//     CLASS##Struct *CLASS;																				\
//     CORE_DebugAssertPointer(CLASS##Handle);																\
//     CLASS = CORE_ClassCast(*CLASS##Handle, CLASS##Struct);												\
//     CLASS##_InternalDelete(CLASS);                                                                      \
//     CLASS->CORE_ClassHandle = (CORE_Handle) NULL; 														\
//     CORE_MemFree(CLASS); 																				\
//     *CLASS##Handle = (CORE_Handle) NULL;																\
//     return TRUE;																						\
// }																										\

#endif