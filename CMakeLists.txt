cmake_minimum_required(VERSION 3.10)

project(ProjectLabyrinth LANGUAGES C VERSION 0.0.0.1)

add_compile_options(-Wall -pedantic)

add_subdirectory(gameserver)
add_subdirectory(CORE)
add_subdirectory(test)

add_executable(ProjectLabyrinthTest test/gameserver/all-tests.c)

target_link_libraries(ProjectLabyrinthTest PUBLIC GameServer CORE GameServerTest)