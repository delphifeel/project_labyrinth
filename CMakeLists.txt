cmake_minimum_required(VERSION 3.21.1)
project(ProjectLabyrinth VERSION 0.3)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set_property(TARGET ProjectLabyrinth PROPERTY CXX_STANDARD 17)

add_compile_options(
	-DCORE_SET_DEBUG_COLORED
)

set(COMMON_INCLUDE 
	"${PROJECT_SOURCE_DIR}"
)

set(THIRD_PARTY ${PROJECT_SOURCE_DIR}/third-party-libraries)
set(LOCAL_COMPLILE_OPTIONS
	-Werror -Wall -Wextra
	$<$<NOT:$<CONFIG:RELEASE>>:
    	-ggdb3 
    	-O0 
    >
    $<$<CONFIG:RELEASE>:
    	-O3
    	-DNDEBUG
    >
)


add_subdirectory(libs)
add_subdirectory(modules)


# third-party libraries 
add_subdirectory(third-party-libraries/libuv)

# Tests

add_executable(ProjectLabyrinthTest test/all-tests.cpp)
target_compile_options(ProjectLabyrinthTest PRIVATE 
	${LOCAL_COMPLILE_OPTIONS}
)
target_include_directories(ProjectLabyrinthTest PUBLIC
	"${COMMON_INCLUDE}"
)
target_link_libraries(ProjectLabyrinthTest PRIVATE
	IOSystem
	# PacketProcessor
	Utils
	Labyrinth
)


# Gameserver
add_executable(GameServer modules/gameserver/main.cpp)
target_compile_options(GameServer PRIVATE 
	${LOCAL_COMPLILE_OPTIONS}
)
target_include_directories(GameServer PUBLIC
	${COMMON_INCLUDE}
)
target_link_libraries(GameServer PRIVATE
	Gameserver
)