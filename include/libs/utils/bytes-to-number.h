#ifndef _BYTES_TO_NUMBER_H_
#define _BYTES_TO_NUMBER_H_

#include "CCORE.h"

int IsLittleEndian(void);

#define BytesToNumber(bytes, bytes_len, variable)  do {     \
    CORE_Assert(sizeof(variable) <= bytes_len);             \
    variable = 0;                                           \
    if (IsLittleEndian()) {                                 \
        for (uint i = 0; i < sizeof(variable); i++) {        \
            variable += bytes[i] << (i * 8);                \
        }                                                   \
    } else {                                                \
        for (uint i = sizeof(variable) - 1; i != 0; i--) {   \
            variable += bytes[i] << (i * 8);                \
        }                                                   \
    }                                                       \
} while (0)

#endif