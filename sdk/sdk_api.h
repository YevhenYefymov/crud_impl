#ifndef SDK_API_H
#define SDK_API_H

#include "sdk_types.h"

// adds object to the object list
// returns object id
uint32_t sdk_create_object(const object_type_t object_type, attr_t* attr_list);

// given the object id, delete the object with such id from the list
void sdk_delete_object(const int id);

// given the object id, read its attributes
void sdk_read_object(const int id, attr_t** attributes);

#endif // SDK_API_H