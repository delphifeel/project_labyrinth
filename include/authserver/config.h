#ifndef _AUTH_SERVER_CONFIG_H_
#define _AUTH_SERVER_CONFIG_H_


static const uint8 mocked_token[32] = 
{
    0xDE, 0xAD, 0xBE, 0xAF, 0x00, 0x01, 0x02, 0x03,
    0xDE, 0xAD, 0xBE, 0xAF, 0x00, 0x01, 0x02, 0x03,
    0xDE, 0xAD, 0xBE, 0xAF, 0x00, 0x01, 0x02, 0x03,
    0xDE, 0xAD, 0xBE, 0xAF, 0x00, 0x01, 0x02, 0x03,
};

/**
 * 	
 * 		AUTH SERVER CONFIG FILE 
 * 
 */

#define TOKEN_SIZE      (32)

/*
 *      Game server network info
 */
#define GAMESERVER_IP_ADDRESS       (127.0.0.1)
#define GAMESERVER_PORT             (7000)

/*
 *      Max users per lobby
 */
// #define LOBBY_USERS_COUNT    (40)
#define LOBBY_USERS_COUNT       (4)


/**
 * 		AuthIOSystem: Default port for client-server communication
 */

#define AUTH_IO_SYSTEM_DEFAULT_PORT (7001)


#endif
