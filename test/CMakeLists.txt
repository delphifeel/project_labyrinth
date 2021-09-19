add_library(GameServerTest 
	gameserver/test-commands-listener.c
	gameserver/test-disjoint-set.c
	gameserver/test-lab-generation.c
	gameserver/test-lab-session.c
)

target_include_directories(GameServerTest PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(GameServerTest PUBLIC GameServer CORE)