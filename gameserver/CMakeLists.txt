add_library(gameserver 
	src/lab-points-map.c 
	src/lab-generation.c 
	src/lab-points-map-reader.c 
	src/lab-session.c 
	src/player.c
	src/commands-listener.c 
	src/game-server.c
)

target_include_directories(gameserver PUBLIC "${PROJECT_SOURCE_DIR}/include")