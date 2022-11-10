
# IOSystem
add_library(IOSystem 
	iosystem/iosystem.cpp
)
target_include_directories(IOSystem PUBLIC
	${COMMON_INCLUDE}
)
target_link_libraries(IOSystem PUBLIC
	TCP
)

# Labyrinth
add_library(Labyrinth 
	labyrinth/lab-generation.cpp
	labyrinth/lab-points-map.cpp
	labyrinth/lab-point.cpp
	labyrinth/lab-session.cpp

	labyrinth/player/player.cpp
)
target_include_directories(Labyrinth PUBLIC
	${COMMON_INCLUDE}
)

# Gameserver
add_library(Gameserver 
	gameserver/gameserver.cpp
	gameserver/packet/packet.cpp
)
target_include_directories(Gameserver PUBLIC
	${COMMON_INCLUDE}
	"${THIRD_PARTY}/json/include"
)
target_link_libraries(Gameserver PUBLIC
	Labyrinth
	IOSystem
	Utils
)