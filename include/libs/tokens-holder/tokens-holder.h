#ifndef _TOKENS_HOLDER_H_
#define _TOKENS_HOLDER_H_

#include "CCORE.h"
#include "modules/player/player-token.h"

typedef struct TokensHolder_s TokensHolder;

typedef struct TokensHolderRecord_s
{
    uint8   Token[PLAYER_TOKEN_SIZE];
    void   *IOStream;
    uint    PlayerId;
    uint    SessionIndex;
} TokensHolderRecord;

bool TokensHolder_Add
(
    TokensHolder                *tokens_holder, 
    const TokensHolderRecord    *record
);
const TokensHolderRecord *TokensHolder_Find 
(
    const TokensHolder       *tokens_holder, 
    const uint8               token[PLAYER_TOKEN_SIZE]
);
bool          TokensHolder_Exist(const TokensHolder *tokens_holder, const uint8 token[PLAYER_TOKEN_SIZE]);

TokensHolder *TokensHolder_Create(uint size);
void          TokensHolder_Free(TokensHolder *tokens_holder);

#endif