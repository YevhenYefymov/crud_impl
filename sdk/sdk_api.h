#ifndef SDK_API_H
#define SDK_API_H

#include "sdk_types.h"

// adds object to the object list
// returns object id
uint32_t sdk_create_object(const object_type_t object_type, attr_t* attr_list, const uint32_t attr_count);

// given the object id, delete the object with such id from the list
operation_result_t sdk_delete_object(const int id);

// given the object id, read its attributes
operation_result_t sdk_read_object(const int id, attr_t** attributes);

// given the object id, update its attributes
operation_result_t sdk_update_object(const int id, attr_t* attributes, const uint32_t attr_count);

#endif // SDK_API_H