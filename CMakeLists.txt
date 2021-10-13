cmake_minimum_required(VERSION 3.10)

project(ProjectLabyrinth LANGUAGES C VERSION 0.0.0.1)

add_compile_options(-Wall -pedantic --std=c99)

add_subdirectory(gameserver)
add_subdirectory(CORE)
add_subdirectory(test)


# third-party libraries 
add_subdirectory(libuv)
include_directories(libuv/include, libuv/src)


add_executable(ProjectLabyrinthTest CORE/src/server.c)

target_link_libraries(ProjectLabyrinthTest PUBLIC GameServer CORE GameServerTest uv)