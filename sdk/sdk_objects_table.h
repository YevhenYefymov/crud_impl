#ifndef SDK_OBJECTS_TABLE_H
#define SDK_OBJECTS_TABLE_H

#include <stdio.h>
#include <stdlib.h>

#include "sdk_types.h"
#include "sdk_object_list.h"

struct objects_table_entry
{
    struct objects_table_entry* next;

    object_type_t   key;
    object_list_t*  objects;
};
typedef struct objects_table_entry objects_table_entry_t;

struct objects_table
{
	objects_table_entry_t* head;
};
typedef struct objects_table objects_table_t;

// add new object to the corresponding entry in the table
uint32_t add_object(const object_type_t key, attr_t* attr_list, const uint32_t attr_count);

// get an object with corresponding id from the list identified by given key
object_list_entry_t* get_object(const object_type_t key, const uint32_t id);

// deletes the object with given id from the corresponding table
operation_result_t delete_object(const object_type_t key, const uint32_t id);

#endif // SDK_OBJECTS_TABLE_H