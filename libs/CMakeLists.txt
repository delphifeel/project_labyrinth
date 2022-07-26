set(COMMON_INCLUDE 
	"${PROJECT_SOURCE_DIR}/include"
	"${THIRD_PARTY}/libCCORE/include"
)


# TCP
add_library(TCP 
	tcp/tcp-client.c
	tcp/tcp-server.c
)
target_include_directories(TCP PUBLIC
	${COMMON_INCLUDE}
	"${THIRD_PARTY}/libuv/include"
	"${THIRD_PARTY}/libuv/src"
)
target_link_libraries(TCP PRIVATE
	uv
)


# Packet
add_library(Packet 
	packet/packet.c
)
target_include_directories(Packet PUBLIC
	${COMMON_INCLUDE}
)


# Utils
add_library(Utils 
	utils/bytes-to-number.c
	utils/chunk-splitter.c
)
target_include_directories(Utils PUBLIC
	${COMMON_INCLUDE}
)
