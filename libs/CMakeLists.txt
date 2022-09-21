set(COMMON_INCLUDE 
	"${PROJECT_SOURCE_DIR}/include"
)


# TCP
add_library(TCP 
	tcp/tcp-client.cpp
	tcp/tcp-server.cpp
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
# add_library(TokensHolder 
# 	tokens-holder/tokens-holder.c
# )
# target_include_directories(TokensHolder PUBLIC
# 	${COMMON_INCLUDE}
# )

# Utils
add_library(Utils 
	utils/bytes-to-number.cpp
	utils/chunk-splitter.cpp
)
target_include_directories(Utils PUBLIC
	${COMMON_INCLUDE}
)
