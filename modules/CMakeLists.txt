
# PacketProcessor
add_library(PacketProcessor 
	packet-processor/packet-processor.cpp
	packet-processor/process-functions/process-functions.cpp
	# All process functions
	packet-processor/process-functions/process-join-lobby.cpp
)
target_include_directories(PacketProcessor PUBLIC
	${COMMON_INCLUDE}
)

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
)
target_include_directories(Gameserver PUBLIC
	${COMMON_INCLUDE}
)
target_link_libraries(Gameserver PUBLIC
	Labyrinth
	IOSystem
	PacketProcessor
	Utils
)