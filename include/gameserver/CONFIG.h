#ifndef _GAME_SERVER_CONFIG_H_
#define _GAME_SERVER_CONFIG_H_


static const uint8 gameserver_mocked_token[32] = 
{
    0xDE, 0xAD, 0xBE, 0xAF, 0x00, 0x01, 0x02, 0x03,
    0xDE, 0xAD, 0xBE, 0xAF, 0x00, 0x01, 0x02, 0x03,
    0xDE, 0xAD, 0xBE, 0xAF, 0x00, 0x01, 0x02, 0x03,
    0xDE, 0xAD, 0xBE, 0xAF, 0x00, 0x01, 0x02, 0x03,
};

/**
 * 	
 * 		GAME SERVER CONFIG FILE 
 * 
 */

/*****************************************************************************************************************************/

/**
 * 		Max sessions count per game server
 *
 *      TODO(delpifeel): bacause of TCP server restrictions 
 *      we can't add more session to single game server instance
 */
#define SESSIONS_CAPACITY		(2)

/*
 * 		Max players per session
 */
// #define SESSION_PLAYERS_COUNT	(40)
#define SESSION_PLAYERS_COUNT       (1)

/*
 * 		CommandsIOSystem: Default port for client-server communication
 */
#define COMMANDS_IO_SYSTEM_DEFAULT_PORT  (7000)


/*****************************************************************************************************************************/

#endif
