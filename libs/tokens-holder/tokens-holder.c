#include "libs/tokens-holder/tokens-holder.h"


typedef struct TokensHolder_s
{
    TokensHolderRecord  *tokens;
    uint                 tokens_len;
    uint                 tokens_cap;
} TokensHolder;


uint TokensHolder_Size(const TokensHolder *tokens_holder)
{
    return tokens_holder->tokens_len;
}

const TokensHolderRecord *TokensHolder_Get(const TokensHolder *tokens_holder, uint index)
{
    if (index + 1 > tokens_holder->tokens_len) {
        CORE_DebugError("Array out of bounds\n");
        return NULL;
    }

    return &tokens_holder->tokens[index];
}

bool TokensHolder_Add
(
    TokensHolder                *tokens_holder, 
    const TokensHolderRecord    *record
)
{
    CORE_AssertPointer(tokens_holder);
    CORE_AssertPointer(record);


    if (tokens_holder->tokens_len == tokens_holder->tokens_cap) {
        return false;
    }

    tokens_holder->tokens[tokens_holder->tokens_len] = *record;
    tokens_holder->tokens_len++;
    return true;
}

const TokensHolderRecord *TokensHolder_Find 
(
    const TokensHolder       *tokens_holder, 
    const uint8               token[PLAYER_TOKEN_SIZE]
)
{
    CORE_AssertPointer(tokens_holder);
    CORE_AssertPointer(token);

    const TokensHolderRecord *record = NULL;

    for (uint i = 0; i < tokens_holder->tokens_len; i++) {
        record = &tokens_holder->tokens[i];
        if (CORE_MemCmp(record->Token, token, sizeof(record->Token))) {
            return record;
        }
    }

    return NULL;
}

bool TokensHolder_Exist(const TokensHolder *tokens_holder, const uint8 token[PLAYER_TOKEN_SIZE])
{
    CORE_AssertPointer(tokens_holder);
    CORE_AssertPointer(token);

    if (!TokensHolder_Find(tokens_holder, token)) {
        return false;
    }

    return true;
}

TokensHolder *TokensHolder_Create(uint size)
{
    CORE_Assert(size > 0);


    TokensHolder *tokens_holder     = CORE_MemAlloc(sizeof(TokensHolder), 1);
    tokens_holder->tokens_cap       = size;
    tokens_holder->tokens_len       = 0;
    tokens_holder->tokens           = CORE_MemAlloc(sizeof(TokensHolderRecord), size);

    return tokens_holder;
}


void TokensHolder_Free(TokensHolder *tokens_holder)
{
    CORE_AssertPointer(tokens_holder);


    CORE_MemFree(tokens_holder->tokens);

    CORE_MemFree(tokens_holder);
}