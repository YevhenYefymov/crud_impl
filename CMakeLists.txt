cmake_minimum_required (VERSION 3.10)
project (CRUD C)

add_subdirectory(sdk)
add_subdirectory(crud_api)

add_executable(main main.c)

target_link_libraries(main crud)