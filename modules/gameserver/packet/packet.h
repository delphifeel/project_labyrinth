#ifndef _PACKET_H_
#define _PACKET_H_

#include "libs/core/core.h"
#include "../player-token.h"

enum PacketType : unsigned int
{
    // TODO: command for testing will be removed
    StartGame = 0,
};


class PacketIn
{
public:
    uint          ValidationHeader;
    uint          Type;
    PlayerToken   Token;
    uint          PayloadSize;
    const uint8  *Payload = nullptr;
public:
    bool FromBuffer(const uint8 buffer[], uint buffer_len);
};

class PacketOut
{
public:
    uint                ValidationHeader;
    uint                Status;
    uint                Type;
    PlayerToken         Token;
    uint                PayloadSize;
    uint8              *Payload;
public:
    uint ToBuffer(uint8 buffer[], uint buffer_size);
};

#endif