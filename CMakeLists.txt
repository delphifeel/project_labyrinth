cmake_minimum_required(VERSION 3.10)
project(ProjectLabyrinth LANGUAGES C VERSION 0.2)

add_compile_options(
	-DCORE_SET_DEBUG_COLORED
)

set(THIRD_PARTY ${PROJECT_SOURCE_DIR}/third-party-libraries)


set(LOCAL_COMPLILE_OPTIONS
	$<$<NOT:$<CONFIG:RELEASE>>:
		-std=c99 -pedantic -Wall 
    	-Wno-missing-braces -Wextra -Wno-missing-field-initializers -Wformat=2 
    	-Wswitch-default -Wswitch-enum -Wcast-align -Wpointer-arith 
    	-Wbad-function-cast -Wstrict-overflow=5 -Wstrict-prototypes -Winline 
    	-Wundef -Wnested-externs -Wcast-qual -Wshadow -Wunreachable-code 
    	-Wlogical-op -Wfloat-equal -Wstrict-aliasing=2 -Wredundant-decls 
    	-Wold-style-definition -Wno-variadic-macros
    	-ggdb3 
    	-O0 
    	-fno-omit-frame-pointer -ffloat-store -fno-common -fstrict-aliasing 
    	-lm
    >
    $<$<CONFIG:RELEASE>:
    	-lm -std=c99 -pedantic 
    	-Wno-variadic-macros
    	-O3
    	-DNDEBUG
     	-funroll-loops
    >
)


add_subdirectory(libs)
add_subdirectory(modules)


# third-party libraries 
add_subdirectory(third-party-libraries/libuv)
add_subdirectory(third-party-libraries/libCCORE)

# Tests

add_executable(ProjectLabyrinthTest test/all-tests.c)
target_compile_options(ProjectLabyrinthTest PRIVATE 
	${LOCAL_COMPLILE_OPTIONS}
)
target_include_directories(ProjectLabyrinthTest PUBLIC
	"${PROJECT_SOURCE_DIR}/include"
	"${THIRD_PARTY}/libCCORE/include"
)
target_link_libraries(ProjectLabyrinthTest PRIVATE
	CCORE
	IOSystem
	PacketProcessor
	Utils
	Labyrinth
	TokensHolder
)


# Gameserver
add_executable(GameServer modules/gameserver/gameserver.c)
target_compile_options(GameServer PRIVATE 
	${LOCAL_COMPLILE_OPTIONS}
)
target_include_directories(GameServer PUBLIC
	"${PROJECT_SOURCE_DIR}/include"
	"${THIRD_PARTY}/libCCORE/include"
)
target_link_libraries(GameServer PRIVATE
	CCORE
	IOSystem
	PacketProcessor
	Utils
	Labyrinth
	TokensHolder
)