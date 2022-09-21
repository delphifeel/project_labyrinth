
# PacketProcessor
# add_library(PacketProcessor 
# 	packet-processor/packet-processor.c
# 	packet-processor/process-functions/process-functions.c
# 	# All process functions
# 	packet-processor/process-functions/process-join-lobby.c
# )
# target_include_directories(PacketProcessor PUBLIC
# 	${COMMON_INCLUDE}
# )

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

# Player
add_library(Player 
	player/player.cpp
)
target_include_directories(Player PUBLIC
	${COMMON_INCLUDE}
)

# Labyrinth
add_library(Labyrinth 
	labyrinth/lab-generation.cpp
	labyrinth/lab-points-map.cpp
	labyrinth/lab-point.cpp
	# labyrinth/lab-session.c
)
target_include_directories(Labyrinth PUBLIC
	${COMMON_INCLUDE}
)
target_link_libraries(Labyrinth PUBLIC
	Player
)
