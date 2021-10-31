#ifndef _GAMESERVER_COMMAND_H_
#define _GAMESERVER_COMMAND_H_

#include "CORE.h"
#include "common.h"
#include "lab-session.h"

/*****************************************************************************************************************************/

#include "gameserver-command-private.h"

typedef struct _GameServerCommand GameServerCommand;

/*****************************************************************************************************************************/

void 		GameServerCommand_GetType(GameServerCommand *instance, uint32 *out_command_type);
void 		GameServerCommand_GetSessionsPtr(GameServerCommand *instance, LabSession *out_sessions_ptr[], uint32 *out_sessions_size);
void 		GameServerCommand_GetSessionIndex(GameServerCommand *instance, uint32 *out_session_index);
void 		GameServerCommand_GetPlayerIndex(GameServerCommand *instance, uint32 *out_player_index);
void 		GameServerCommand_GetPlayerTokenPtr(GameServerCommand *instance, const uint8 *out_player_token_ptr[TOKEN_SIZE]);
void 		GameServerCommand_GetPayloadPtr(GameServerCommand *instance, const uint8 **out_payload_ptr, uint32 *out_payload_size);

void 		GameServerCommand_SetType(GameServerCommand *instance, uint32 command_type);
void 		GameServerCommand_SetSessionsPtr(GameServerCommand *instance, LabSession *sessions_ptr, uint32 session_size);
void 		GameServerCommand_SetSessionIndex(GameServerCommand *instance, uint32 session_index);
void 		GameServerCommand_SetPlayerIndex(GameServerCommand *instance, uint32 player_index);
void 		GameServerCommand_SetPlayerToken(GameServerCommand *instance, const uint8 player_token[TOKEN_SIZE]);
CORE_Bool 	GameServerCommand_SetPayload(GameServerCommand *instance, const uint8 payload[], uint32 payload_size);

void 		GameServerCommand_Init(GameServerCommand *instance);

#endif
