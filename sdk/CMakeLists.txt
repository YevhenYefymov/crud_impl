add_library(sdk sdk_api.c sdk_object_list.c sdk_objects_table.c)

target_include_directories(sdk PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})