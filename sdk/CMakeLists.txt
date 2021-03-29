add_library(sdk sdk_api.c sdk_object_list.c)

target_include_directories(sdk PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})