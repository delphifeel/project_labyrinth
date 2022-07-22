add_library(TCP 
	tcp/tcp-client.c
	tcp/tcp-server.c
)
target_include_directories(TCP PUBLIC
	"${PROJECT_SOURCE_DIR}/include"
	"${THIRD_PARTY}/libuv/include"
	"${THIRD_PARTY}/libuv/src"
	"${THIRD_PARTY}/libCCORE/include"
)
target_link_libraries(TCP PRIVATE
	uv
)


add_library(Packet 
	packet/packet.c
)
target_include_directories(Packet PUBLIC
	"${PROJECT_SOURCE_DIR}/include"
	"${THIRD_PARTY}/libCCORE/include"
)