set(COMMON_INCLUDE 
	"${PROJECT_SOURCE_DIR}/include"
	"${THIRD_PARTY}/libCCORE/include"
)

# InputProcessor
add_library(InputProcessor 
	input-processor/input-processor.c
)
target_include_directories(InputProcessor PUBLIC
	${COMMON_INCLUDE}
)

# IOSystem
add_library(IOSystem 
	iosystem/iosystem.c
)
target_include_directories(IOSystem PUBLIC
	${COMMON_INCLUDE}
)
target_link_libraries(IOSystem PUBLIC
	TCP
)

# Player
add_library(Player 
	player/player.c
)
target_include_directories(Player PUBLIC
	${COMMON_INCLUDE}
)

# Labyrinth
add_library(Labyrinth 
	labyrinth/lab-generation.c
	labyrinth/lab-points-map.c
	labyrinth/lab-session.c
)
target_include_directories(Labyrinth PUBLIC
	${COMMON_INCLUDE}
)
target_link_libraries(Labyrinth PUBLIC
	CCORE
	Player
)
