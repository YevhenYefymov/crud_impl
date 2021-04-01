#include "sdk_api.h"
#include "sdk_objects_table.h"
#include <stdio.h>
#include <string.h>

uint32_t sdk_create_object(const object_type_t object_type, attr_t* attr_list, const uint32_t attr_count)
{
    return add_object(object_type, attr_list, attr_count);
}

operation_result_t sdk_delete_object(const object_type_t object_type, const int id)
{
    return delete_object(object_type, id);
}

operation_result_t sdk_read_object(const object_type_t object_type, const int id, attr_t** attributes)
{
    if (attributes == 0)
    {
        printf("sdk_read_object: attributes == 0\n");
        return RSLT_FAILURE;
    }

    object_list_entry_t* object = get_object(object_type, id);
    if (object)
    {
        *attributes = object->attr_list;
        return RSLT_SUCCESS;
    }

    printf("sdk_read_object: object is not found\n");
    return RSLT_FAILURE;
}

// TODO: move this function somewhere else
void set_attribute_value(const attr_type_t type, const attr_value_t input, attr_value_t* value_to_set)
{
    if (value_to_set == 0)
    {
        return;
    }

    switch (type)
    {
        case SDK_OBJ_SWITCH_ATTR_NAME:
            strncpy(value_to_set->chardata, input.chardata, 32);
            break;
        case SDK_OBJ_SWITCH_ATTR_HASH_SEED:
        case SDK_OBJ_SWITCH_ATTR_SPLIT_MODE:
        case SDK_OBJ_SWITCH_ATTR_MAX_PORTS:
            value_to_set->u32 = input.u32;
            break;

        // port attributes 
        case SDK_OBJ_PORT_ATTR_STATE:
            value_to_set->booldata = input.booldata;
            break;
        case SDK_OBJ_PORT_ATTR_SPEED:
        case SDK_OBJ_PORT_ATTR_IPV4:
        case SDK_OBJ_PORT_ATTR_MTU:
            value_to_set->u32 = input.u32;
            break;
        
        default:
            // don't set anything
            break;
    }
}

operation_result_t sdk_update_object(const object_type_t object_type, const int id, attr_t* attributes, const uint32_t attr_count)
{
    if (attributes == 0)
    {
        printf("sdk_update_ojbect: attributes = 0\n");
        return RSLT_FAILURE;
    }

    object_list_entry_t* object = get_object(object_type, id);
    if (object)
    {
        int i;
        for (i = 0; i < attr_count; ++i)
        {
            int j;
            for (j = 0; j < object->attr_count; ++j)
            {
                if (object->attr_list[j].id == attributes[i].id)
                {
                    set_attribute_value(attributes[i].id, attributes[i].value, &object->attr_list[j].value);
                }
            }
        }

        return RSLT_SUCCESS;
    }
    printf("sdk_update_object: object is not found\n");
    return RSLT_FAILURE;
}