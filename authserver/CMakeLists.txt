add_library(AuthServer 
	src/auth-io-system.c
	src/message.c
)

target_include_directories(AuthServer PUBLIC "${PROJECT_SOURCE_DIR}/include")