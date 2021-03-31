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

operation_result_t sdk_delete_object(const int id)
{
    object_list_t* object_list = get_object_list();
    return delete_node(object_list, id);
}

operation_result_t sdk_read_object(const int id, attr_t** attributes)
{
    if (attributes == 0)
    {
        printf("sdk_read_object: attributes == 0\n");
        return RSLT_FAILURE;
    }

    object_list_t* object_list = get_object_list();
    if (object_list == 0)
    {
        printf("sdk_read_object: object_list == 0\n");
        return RSLT_FAILURE;
    }

    object_list_entry_t* object = get_node(get_object_list(), id);
    if (object)
    {
        *attributes = object->attr_list;
        return RSLT_SUCCESS;
    }
    printf("sdk_read_object: object is not found\n");
    return RSLT_FAILURE;
}

operation_result_t sdk_update_object(const int id, attr_t* attributes)
{
    if (attributes == 0)
    {
        printf("sdk_update_ojbect: attributes = 0\n");
        return RSLT_FAILURE;
    }

    object_list_t* object_list = get_object_list();
    if (object_list == 0)
    {
        printf("sdk_update_ojbect: object list does not exist\n");
        return RSLT_FAILURE;
    }
    object_list_entry_t* object = get_node(object_list, id);
    if (object)
    {
        free(object->attr_list);

        object->attr_list = attributes;
        return RSLT_SUCCESS;
    }
    printf("sdk_update_object: object is not found\n");
    return RSLT_FAILURE;
}