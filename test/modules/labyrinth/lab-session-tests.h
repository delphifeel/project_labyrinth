#include "libs/core/core.h"
#include "modules/labyrinth/lab-session.h"

static void Test_LabSessionAddPlayers(void)
{
    LabSession session;
    session.Setup(2);

    auto [ player1_id, ok_1 ] = session.AddPlayer();
    auto [ player2_id, ok_2 ] = session.AddPlayer();

    CORE_AssertIntEqual(player1_id, 1);
    CORE_AssertIntEqual(player2_id, 2);

    auto [ player1, player1_ok ] = session.FindPlayer(player1_id);
    auto [ player2, player2_ok ] = session.FindPlayer(player2_id);

    CORE_AssertIntEqual(player1->GetId(), player1_id);
    CORE_AssertIntEqual(player2->GetId(), player2_id);
}

void LabSession_TestsRun(void)
{
    Test_LabSessionAddPlayers();
    // Test_LabSessionLabyrinthToJSON();
}