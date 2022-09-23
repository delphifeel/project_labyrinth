#include <map>
#include "modules/labyrinth/player/player-token.h"

static void _TokensHolderWorksCorrectly(void)
{
    std::map<const PlayerToken, TokenRecord> tokens_holder;

    PlayerToken token1 = { 0xBA, 0xDB, 0xEE };
    CORE_Assert(tokens_holder.count(token1) == 0);

    // add token 1
    TokenRecord token1_record = {
                  token1,           // Token
         (void *) 0xAABBCCDD,       // IOStream
                  1,                // PlayerId
                  0,                // SessionIndex
    };

    tokens_holder[token1] = token1_record;
    CORE_Assert(tokens_holder.count(token1) == 1);
    auto& record = tokens_holder[token1];
    CORE_Assert(record.IOStream == token1_record.IOStream);
    CORE_AssertIntEqual(record.PlayerId, token1_record.PlayerId);
    CORE_AssertIntEqual(record.SessionIndex, token1_record.SessionIndex);

    // add token 2
    PlayerToken token2 = { 0xDE, 0xAD, 0xBE, 0x0E };

    TokenRecord token2_record = {
                  token2,           // Token
         (void *) 0xDDCCBBAA,       // IOStream
                  2,                // PlayerId
                  1,                // SessionIndex
    };

    tokens_holder[token2] = token2_record;
    CORE_Assert(tokens_holder.count(token1) == 1);
    CORE_Assert(tokens_holder.count(token2) == 1);
    auto& record2 = tokens_holder[token2];
    CORE_Assert(record2.IOStream == token2_record.IOStream);
    CORE_AssertIntEqual(record2.PlayerId, token2_record.PlayerId);
    CORE_AssertIntEqual(record2.SessionIndex, token2_record.SessionIndex);
}

void TokensHolder_TestsRun(void)
{
    _TokensHolderWorksCorrectly();
}