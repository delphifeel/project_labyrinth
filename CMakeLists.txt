cmake_minimum_required(VERSION 3.10)
project(ProjectLabyrinth LANGUAGES C VERSION 0.0.0.1)
add_compile_options(
	-Wall 
	-pedantic 
	--std=gnu99 
	-D CORE_SET_TESTING_ENABLED
)


add_subdirectory(gameserver)
add_subdirectory(authserver)
add_subdirectory(CORE)
add_subdirectory(test)


# third-party libraries 
add_subdirectory(third-party-libraries/libuv)


add_executable(ProjectLabyrinthTest test/gameserver/all-tests.c)

target_link_libraries(ProjectLabyrinthTest PUBLIC 
	CORE 
	GameServer
	GameServerTest
	AuthServer
	AuthServerTest 
	CORETest 
	uv
)