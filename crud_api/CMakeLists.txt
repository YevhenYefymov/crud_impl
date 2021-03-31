add_subdirectory(private)

add_library(crud private/crud.c private/crud_helper.c private/types_converter.c)

target_link_libraries (crud LINK_PUBLIC sdk)