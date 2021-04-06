#ifndef SDK_OBJECT_LIST_H
#define SDK_OBJECT_LIST_H

#include <stdio.h>
#include <stdlib.h>

#include "sdk_types.h"

struct object_list_entry
{
    struct object_list_entry* next;

    attr_t*         attr_list;
    uint32_t        attr_count;
    uint32_t        id;
};
typedef struct object_list_entry object_list_entry_t;

struct object_list
{
	object_list_entry_t* head;
    uint32_t size;
};
typedef struct object_list object_list_t;

// creates default list
// returns pointer to newly created list
object_list_t* create_list();

// deletes given list and all of it's nodes, freeing the memory
operation_result_t delete_list(object_list_t* list);

// adds node to the list
// returns id of the added entry
uint32_t add_node(object_list_t* list, attr_t* attr_list, const uint32_t attr_count);

// finds node with given id and deletes it from the list
// TODO: return result of the operation
operation_result_t delete_node(object_list_t* list, const int id);

// get node by it's ID
// return found node or 0 if not found
object_list_entry_t* get_node(object_list_t* list, const int id);

#endif // SDK_OBJECT_LIST_H