#ifndef _GAMESERVER_COMMAND_H_
#define _GAMESERVER_COMMAND_H_

#include "CORE.h"
#include "common.h"
#include "lab-session.h"

/*****************************************************************************************************************************/

#include "gameserver-command-private.h"

/*****************************************************************************************************************************/

void 		GameServerCommand_GetType(struct GameServerCommand *instance, uint32 *out_command_type);
void 		GameServerCommand_GetSessionsPtr(struct GameServerCommand *instance, LabSession *out_sessions_ptr[], uint32 *out_sessions_size);
void 		GameServerCommand_GetSessionIndex(struct GameServerCommand *instance, uint32 *out_session_index);
void 		GameServerCommand_GetPlayerIndex(struct GameServerCommand *instance, uint32 *out_player_index);
void 		GameServerCommand_GetPlayerTokenPtr(struct GameServerCommand *instance, const uint8 *out_player_token_ptr[TOKEN_SIZE]);
void 		GameServerCommand_GetPayloadPtr(struct GameServerCommand *instance, const uint8 **out_payload_ptr, uint32 *out_payload_size);

void 		GameServerCommand_SetType(struct GameServerCommand *instance, uint32 command_type);
void 		GameServerCommand_SetSessionsPtr(struct GameServerCommand *instance, LabSession *sessions_ptr, uint32 sessions_size);
void 		GameServerCommand_SetSessionIndex(struct GameServerCommand *instance, uint32 session_index);
void 		GameServerCommand_SetPlayerIndex(struct GameServerCommand *instance, uint32 player_index);
void 		GameServerCommand_SetPlayerToken(struct GameServerCommand *instance, const uint8 player_token[TOKEN_SIZE]);
CORE_Bool 	GameServerCommand_SetPayload(struct GameServerCommand *instance, const uint8 payload[], uint32 payload_size);

void 		GameServerCommand_Init(struct GameServerCommand *instance);

#endif
