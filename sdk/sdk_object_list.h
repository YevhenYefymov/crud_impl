#ifndef SDK_OBJECT_LIST_H
#define SDK_OBJECT_LIST_H

#include <stdio.h>
#include <stdlib.h>

#include "sdk_types.h"

struct object_list_entry
{
    struct object_list_entry* next;

    object_type_t   object_type;
    attr_t*         attr_list;
    uint32_t        id;
}; 
typedef struct object_list_entry object_list_entry_t;

struct object_list
{
	object_list_entry_t* head;
};
typedef struct object_list object_list_t;

// returns current object list
object_list_t* get_object_list();

// creates default list
// returns pointer to newly created list
object_list_t* create_list();

// adds node to the list
// returns id of the added entry
uint32_t add_node(object_list_t* list, const object_type_t object_type, attr_t* attr_list);

// finds node with given id and deletes it from the list
// TODO: return result of the operation
operation_result_t delete_node(object_list_t* list, const int id);

// get node by it's ID
// return found node or 0 if not found
object_list_entry_t* get_node(object_list_t* list, const int id);

#endif // SDK_OBJECT_LIST_H