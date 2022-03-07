#ifndef _CORE_ARRAY_H_
#define _CORE_ARRAY_H_

#include "CORE-types.h"
#include "CORE-debug.h"
#include "CORE-safe-array-private.h"


#define CSafeArray_From(NAME, CAPACITY, SIZE)                               \
    const uint32    _CARR_VAR(NAME, capacity) = CAPACITY;                   \
    uint32          _CARR_VAR(NAME, size) = SIZE

#define CSafeArray_Create(NAME, TYPE, CAPACITY)                             \
    TYPE           NAME[CAPACITY];                                          \
    const uint32   _CARR_VAR(NAME, capacity) = CAPACITY;                    \
    uint32         _CARR_VAR(NAME, size) = 0                    

#define CSafeArray_Clear(NAME)                                              \
    _CARR_VAR(NAME, size) = 0                                     

#define CSafeArray_Add(NAME, VALUE)                                         \
    _CSafeArray_CheckBounds(NAME, _CARR_VAR(NAME, size));                   \
    NAME[_CARR_VAR(NAME, size)++] = VALUE          

#define CSafeArray_Get(NAME, INDEX)                                         \
    (_CSafeArray_CheckBounds(NAME, INDEX), NAME[INDEX])    

#define CSafeArray_GetPtr(NAME, INDEX)                                      \
    (_CSafeArray_CheckBounds(NAME, INDEX), &NAME[INDEX])                 

#define CSafeArray_Change(NAME, INDEX, VALUE)                               \
    _CSafeArray_CheckBounds(NAME, INDEX);                                   \
    NAME[INDEX] = VALUE                               

#define CSafeArray_Size(NAME)               (_CARR_VAR(NAME, size))
#define CSafeArray_Capacity(NAME)           (_CARR_VAR(NAME, capacity))


#endif