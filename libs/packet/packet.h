#ifndef _PACKETS_H_
#define _PACKETS_H_

#include "libs/core/core.h"

struct Packet
{
    uint          Type;
    uint          SessionIndex;
    uint          PlayerId;
    uint          PayloadSize;
    const uint8  *Payload;
};

#endif