#ifndef _COMMANDS_PROCESSOR_H_
#define _COMMANDS_PROCESSOR_H_

#include "command.h"
#include "lab-session.h"

CORE_Bool CommandsProcessor_Process(Command *command, LabSession sessions[], uint32 sessions_size);

#endif