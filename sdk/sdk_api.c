#include "sdk_api.h"
#include "sdk_object_list.h"
#include <stdio.h>

uint32_t sdk_create_object(const object_type_t object_type, attr_t* attr_list)
{
    object_list_t* object_list = get_object_list();
    if (object_list == 0)
    {
        object_list = create_list();
    }
    uint32_t id = add_node(object_list, object_type, attr_list);
    return id;
}

void sdk_delete_object(const int id)
{
    // TODO: return operation result - list might not exist or such entry might not be in it
    object_list_t* object_list = get_object_list();
    delete_node(object_list, id);
}


void sdk_read_object(const int id, attr_t* attributes)
{
    printf("searching for entry with id %d\n", id);
    object_list_t* object_list = get_object_list();
    if (object_list)
    {
        object_list_entry_t* object = get_node(get_object_list(), id);
        if (object != 0)
        {
            attributes = object->attr_list;
        }
    }
}