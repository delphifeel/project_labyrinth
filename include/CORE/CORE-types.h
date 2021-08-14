#ifndef __CORE_TYPES_H__
#define __CORE_TYPES_H__

#include <stddef.h>

#define TRUE                                                    (1)
#define FALSE                                                   (0)

typedef unsigned char                                           uint8;                              /* unsigned */
typedef unsigned short int                                      uint16;
typedef unsigned long int                                       uint32;
typedef unsigned long long                                      uint64;
typedef signed char                                             int8;                               /* signed */
typedef signed short int                                        int16;
typedef signed long int                                         int32;
typedef signed long long                                        int64;
typedef float                                                   float32;                            /* single precision floating point */
typedef double                                                  float64;                            /* double precision floating point */
typedef signed long int 										CORE_Bool;

#endif