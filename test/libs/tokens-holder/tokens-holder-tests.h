#include "libs/tokens-holder/tokens-holder.h"

static void _TokensHolderWorksCorrectly(void)
{
    const TokensHolderRecord *record    = NULL;
    const uint max_size                 = 10;
    TokensHolder *tokens_holder         = TokensHolder_Create(max_size);

    uint8  token1[PLAYER_TOKEN_SIZE];
    uint32 token1_value = 0xBADBEE;
    CORE_MemZero(token1, sizeof(token1));
    CORE_MemCpy(token1, &token1_value, sizeof(token1_value));

    CORE_Assert(TokensHolder_Exist(tokens_holder, token1) == false);
    CORE_Assert(TokensHolder_Find(tokens_holder, token1) == NULL);

    // add token 1
    TokensHolderRecord token1_record = {
        .IOStream       = (void *) 0xAABBCCDD,
        .PlayerId       = 1,
        .SessionIndex   = 0,
    };
    CORE_MemCpy(token1_record.Token, token1, sizeof(token1_record.Token));

    TokensHolder_Add(tokens_holder, &token1_record);
    CORE_Assert(TokensHolder_Exist(tokens_holder, token1) == true);
    record = TokensHolder_Find(tokens_holder, token1);
    CORE_Assert(record->IOStream == token1_record.IOStream);
    CORE_AssertIntEqual(record->PlayerId, token1_record.PlayerId);
    CORE_AssertIntEqual(record->SessionIndex, token1_record.SessionIndex);

    // add token 2
    uint8  token2[PLAYER_TOKEN_SIZE];
    uint32 token2_value = 0xDEADBEE;
    CORE_MemZero(token2, sizeof(token2));
    CORE_MemCpy(token2, &token2_value, sizeof(token2_value));

    TokensHolderRecord token2_record = {
        .IOStream       = (void *) 0xDDCCBBAA,
        .PlayerId       = 2,
        .SessionIndex   = 1,
    };
    CORE_MemCpy(token2_record.Token, token2, sizeof(token2_record.Token));

    TokensHolder_Add(tokens_holder, &token2_record);
    CORE_Assert(TokensHolder_Exist(tokens_holder, token2) == true);
    CORE_Assert(TokensHolder_Exist(tokens_holder, token1) == true);
    record = TokensHolder_Find(tokens_holder, token2);
    CORE_Assert(record->IOStream == token2_record.IOStream);
    CORE_AssertIntEqual(record->PlayerId, token2_record.PlayerId);
    CORE_AssertIntEqual(record->SessionIndex, token2_record.SessionIndex);

    TokensHolder_Free(tokens_holder);
}

void TokensHolder_TestsRun(void)
{
    _TokensHolderWorksCorrectly();
}