#ifndef _TOKEN_RECORD_H_
#define _TOKEN_RECORD_H_

#include "core/core.h"
#include "modules/player/player-token.h"

struct TokenRecord
{
    PlayerToken   Token;
    void         *IOStream;
    uint          PlayerId;
    uint          SessionIndex;
};

#endif