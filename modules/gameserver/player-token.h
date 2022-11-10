#ifndef _PLAYER_TOKEN_H_
#define _PLAYER_TOKEN_H_

#include <array>
#include "libs/core/core.h"

constexpr uint kPlayerTokenSize = 32;
using PlayerToken = std::string;

struct PlayerTokenRecord
{
    void         *IOStream;
    uint          PlayerId;
    uint          SessionIndex;
};

#endif