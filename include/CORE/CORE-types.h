#ifndef _CORE_TYPES_H_
#define _CORE_TYPES_H_

#include <stddef.h>
#include <stdint.h>

#define TRUE                                                    (1)
#define FALSE                                                   (0)

typedef uint8_t                                       uint8;                              /* unsigned */
typedef uint16_t                                      uint16;
typedef uint32_t                                      uint32;
typedef uint64_t                                      uint64;
typedef int8_t                                        int8;                               /* signed */
typedef int16_t                                       int16;
typedef int32_t                                       int32;
typedef int64_t                                       int64;
typedef float                                         float32;                            /* single precision floating point */
typedef double                                        float64;                            /* double precision floating point */
typedef int32_t 									  CORE_Bool;

#endif
