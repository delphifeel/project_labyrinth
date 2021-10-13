cmake_minimum_required(VERSION 3.10)

project(ProjectLabyrinth LANGUAGES C VERSION 0.0.0.1)

add_compile_options(-Wall -pedantic --std=c99)

add_subdirectory(gameserver)
add_subdirectory(CORE)
add_subdirectory(test)


# third-party libraries 
add_subdirectory(third-party-libraries/libuv)
include_directories(third-party-libraries/libuv, third-party-libraries/libuv)


add_executable(ProjectLabyrinthTest test/CORE/all-tests.c)
target_link_libraries(ProjectLabyrinthTest PUBLIC GameServer CORE GameServerTest CORETest uv)