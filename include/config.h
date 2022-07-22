#ifndef _CONFIG_H_
#define _CONFIG_H_

/**
 *      Max sessions count per game server
 *
 *      TODO(delpifeel): bacause of TCP server restrictions 
 *      we can't add more session to single game server instance
 */
#define SESSIONS_CAPACITY       (2)

/*
 *      Max players per session
 */
// #define SESSION_PLAYERS_COUNT    (40)
#define SESSION_PLAYERS_COUNT       (2)

#define IOSYSTEM_DEFAULT_PORT       (7000)

#endif