#ifndef _ARRAY_H_
#define _ARRAY_H_

#include "CCORE.h"

const void *PtrsArray_FindImpl(const void *const *array, uint array_size, uint index);

#define PtrsArray_Find(elem_type, array, array_size, index)   \
    ((const elem_type *) PtrsArray_FindImpl((const void *const *) array, array_size, index))

#endif