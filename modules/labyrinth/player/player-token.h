#ifndef _PLAYER_TOKEN_H_
#define _PLAYER_TOKEN_H_

#include <array>
#include "libs/core/core.h"

constexpr uint kPlayerTokenSize = 32;
using PlayerToken = std::array<uint8, kPlayerTokenSize>;

struct TokenRecord
{
    PlayerToken   Token;
    void         *IOStream;
    uint          PlayerId;
    uint          SessionIndex;
};

#endif