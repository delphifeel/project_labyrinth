#include "libs/utils/array.h"

const void *PtrsArray_FindImpl(const void *const *array, uint array_size, uint index)
{
    CORE_AssertPointer(array);

    if (index + 1 > array_size) {
        CORE_DebugError("Array out of bounds\n");
        return NULL;
    }

    return array[index];
}
