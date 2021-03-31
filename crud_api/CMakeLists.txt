add_library(crud crud.c crud_helper.c types_converter.c)

target_link_libraries (crud LINK_PUBLIC sdk)