#ifndef _CORE_ARRAY_H_
#define _CORE_ARRAY_H_

#include "CORE-types.h"
#include "CORE-debug.h"
#include "CORE-array-private.h"


#define CORE_StaticArray_Wrap(NAME, CAPACITY, SIZE)                         \
    const uint32    _CARR_VAR(NAME, capacity) = CAPACITY;                   \
    uint32          _CARR_VAR(NAME, size) = SIZE

#define CORE_StaticArray(NAME, TYPE, CAPACITY)                              \
    TYPE           NAME[CAPACITY];                                          \
    const uint32   _CARR_VAR(NAME, capacity) = CAPACITY;                    \
    uint32         _CARR_VAR(NAME, size) = 0                    

#define CORE_Array_Clear(NAME)                                              \
    _CARR_VAR(NAME, size) = 0                                     

#define CORE_Array_Add(NAME, VALUE)                                         \
    _CORE_Array_CheckBounds(NAME, _CARR_VAR(NAME, size));                   \
    NAME[_CARR_VAR(NAME, size)++] = VALUE          

#define CORE_Array_Get(NAME, INDEX)                                         \
    (_CORE_Array_CheckBounds(NAME, INDEX), NAME[INDEX])              

#define CORE_Array_Change(NAME, INDEX, VALUE)                               \
    _CORE_Array_CheckBounds(NAME, INDEX);                                   \
    NAME[INDEX] = VALUE                               

#define CORE_Array_Size(NAME)               (_CARR_VAR(NAME, size))
#define CORE_Array_Capacity(NAME)           (_CARR_VAR(NAME, capacity))


#endif