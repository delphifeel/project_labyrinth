#ifndef _COMMAND_PLAYER_MOVE_H_
#define _COMMAND_PLAYER_MOVE_H_

#include "CORE.h"
#include "../lab-session.h"
#include "../command.h"

CORE_Bool CommandPlayerMove_Process(Command *command, LabSession sessions[], uint32 sessions_size);

#endif
