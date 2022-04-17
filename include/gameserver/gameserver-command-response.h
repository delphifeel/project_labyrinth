#ifndef _GAMESERVER_COMMAND_RESPONSE_H_
#define _GAMESERVER_COMMAND_RESPONSE_H_

#include "CCORE.h"

/*****************************************************************************************************************************/

#include "gameserver-command-response-private.h"

/*****************************************************************************************************************************/

void    GameServerCommandResponse_GetType(struct GameServerCommandResponse *instance, uint32 *out_command_type);

void    GameServerCommandResponse_GetPlayerIndexesPtr(struct GameServerCommandResponse  *instance, 
                                                      const uint32                      **out_player_indexes_ptr,
                                                      uint32                            *out_player_indexes_size);

void    GameServerCommandResponse_GetPayloadPtr(struct GameServerCommandResponse    *instance, 
                                                const uint8                         **out_payload_ptr, 
                                                uint32                              *out_payload_size);

void    GameServerCommandResponse_SetType(struct GameServerCommandResponse *instance, uint32 command_type);

bool    GameServerCommandResponse_AddPlayerIndex(struct GameServerCommandResponse *instance, 
                                                 uint32                           player_index);

bool    GameServerCommandResponse_SetPayload(struct GameServerCommandResponse   *instance, 
                                             const uint8                        payload[], 
                                             uint32                             payload_size);

void    GameServerCommandResponse_Init(struct GameServerCommandResponse *instance);

#endif
