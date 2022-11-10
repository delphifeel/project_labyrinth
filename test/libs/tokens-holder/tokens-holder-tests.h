#include <map>
#include "modules/gameserver/player-token.h"

static void _TokensHolderWorksCorrectly(void)
{
    std::map<const PlayerToken, PlayerTokenRecord> tokens_holder;

    PlayerToken token1 = "BADBEE";
    CORE_Assert(tokens_holder.count(token1) == 0);

    // add token 1
    PlayerTokenRecord token1_record = {
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
    PlayerToken token2 = "DEADBEE";

    PlayerTokenRecord token2_record = {
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