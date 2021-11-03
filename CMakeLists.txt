cmake_minimum_required(VERSION 3.10)
project(ProjectLabyrinth LANGUAGES C VERSION 0.0.0.1)
add_compile_options(
	-Wall 
	-pedantic 
	--std=gnu99
	#-DCORE_SET_TESTING_ENABLED
	-DCORE_SET_DEBUG_COLORED
)


add_subdirectory(gameserver)
add_subdirectory(authserver)
add_subdirectory(CORE)
add_subdirectory(test)
add_subdirectory(lib)


# third-party libraries 
add_subdirectory(third-party-libraries/libuv)


add_executable(ProjectLabyrinthTest test/all-tests.c)
target_link_libraries(ProjectLabyrinthTest PUBLIC 
	CORETests
	GameServerTests
	AuthServerTests
)

add_executable(ProjectLabyrinthAuthServer authserver/src/main.c)
target_include_directories(ProjectLabyrinthAuthServer PUBLIC 
	"${PROJECT_SOURCE_DIR}/include"
	"${PROJECT_SOURCE_DIR}/lib/commands-processor/include"
)
target_link_libraries(ProjectLabyrinthAuthServer PUBLIC 
	AuthServer
)
