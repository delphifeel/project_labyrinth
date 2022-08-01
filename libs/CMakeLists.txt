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

# TokensHolder
add_library(TokensHolder 
	tokens-holder/tokens-holder.c
)
target_include_directories(TokensHolder PUBLIC
	${COMMON_INCLUDE}
)

# Utils
add_library(Utils 
	utils/bytes-to-number.c
	utils/chunk-splitter.c
	utils/array.c
)
target_include_directories(Utils PUBLIC
	${COMMON_INCLUDE}
)
