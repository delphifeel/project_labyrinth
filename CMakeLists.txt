cmake_minimum_required(VERSION 3.10)
project(ProjectLabyrinth LANGUAGES C VERSION 0.0.0.1)

add_compile_options(
	-DCORE_SET_DEBUG_COLORED
	#-DCORE_SET_TESTING_ENABLED
)


set(LOCAL_COMPLILE_OPTIONS
	$<$<NOT:$<CONFIG:RELEASE>>:
		-std=c89 -pedantic -Wall 
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
    	-lm -std=c89 -pedantic 
    	-Wno-variadic-macros
    	-O3
    	-DNDEBUG
     	-funroll-loops
    >
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
)
target_link_libraries(ProjectLabyrinthAuthServer PUBLIC 
	AuthServer
)

add_executable(ProjectLabyrinthGameServer gameserver/src/main.c)
target_include_directories(ProjectLabyrinthGameServer PUBLIC 
	"${PROJECT_SOURCE_DIR}/include"
)
target_link_libraries(ProjectLabyrinthGameServer PUBLIC 
	GameServer
)
target_compile_options(ProjectLabyrinthGameServer PRIVATE ${LOCAL_COMPLILE_OPTIONS})
target_compile_options(ProjectLabyrinthAuthServer PRIVATE ${LOCAL_COMPLILE_OPTIONS})
target_compile_options(ProjectLabyrinthTest PRIVATE 
	${LOCAL_COMPLILE_OPTIONS}
)