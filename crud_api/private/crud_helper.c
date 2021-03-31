#include "crud_helper.h"
#include "types_converter.h"

#include <stdio.h>
#include <stdlib.h>

/***************** HELPER FUNCTIONS DEFINITION *****************/
int is_attribute_valid(const crud_attribute_t attr)
{
    // TODO: extract functions for switch and port
    switch (attr.id)
    {    
        // switch attributes
        case CRUD_SWITCH_ATTR_NAME:
            return 1;       
        case CRUD_SWITCH_ATTR_HASH_SEED:
        {
            if (attr.value.u32 < 999 /* TODO: define MAX value for hash seed */)
            {
                return 1;
            }
        }
        case CRUD_SWITCH_ATTR_SPLIT_MODE:
        {
            if (attr.value.u32 < 3)
            {
                return 1;
            }
        }
        case CRUD_SWITCH_ATTR_MAX_PORTS:
        {
            if ((attr.value.u32 > 1) && (attr.value.u32 < 32))
            {
                return 1;
            }
        }

        // port attributes 
        case CRUD_PORT_ATTR_STATE: 
            return 1;
        case CRUD_PORT_ATTR_SPEED:
        {
            if ( (attr.value.u32 == 10) || (attr.value.u32 == 100) || (attr.value.u32 == 1000) )
            {
                return 1;
            }
        }
        case CRUD_PORT_ATTR_IPV4:
            return 1;
        case CRUD_PORT_ATTR_MTU:
        {
            if ( (attr.value.u32 > 64) && (attr.value.u32 < 9000) )
            {
                return 1;
            }
        }

        default:
            return 0;
    }

    return 0;
}

int is_attribute_list_valid(crud_attribute_t* attr_list, const uint32_t attr_count)
{
    // check if attribute list contains attributes applicable to different objects
    if (is_attribute_type_present(attr_list, attr_count, is_switch_attribute) && is_attribute_type_present(attr_list, attr_count, is_port_attribute))
    {
        return 0;
    }

    uint32_t i;
    for (i = 0; i < attr_count; ++i)
    {
        if (is_attribute_valid(attr_list[i]) != 1)
        {
            printf("is_attribute_list_valid: at least 1 attribute is invalid\n");
            return 0;
        }
    }
    return 1;
}

int is_attribute_read_only(const crud_attr_id_t attr_id)
{
    switch (attr_id)
    {
        // switch attributes
        case CRUD_SWITCH_ATTR_SPLIT_MODE: 
        case CRUD_SWITCH_ATTR_MAX_PORTS:
            return 1;
        
        // port attributes
        case CRUD_PORT_ATTR_MTU:
            return 1;
        default:
            return 0;
    }
}

// TODO: change this method - take attribute list, seach port_state and port_speed in it, 
// and only then check the condition
int is_port_speed_read_only(const crud_attribute_t port_state_attr)
{
    if (CRUD_PORT_ATTR_STATE != port_state_attr.id)
    {
        return 0;
    }
    
    return (port_state_attr.value.booldata == true);
}

int is_read_only_attribute_present(crud_attribute_t* attr_list, const uint32_t attr_count)
{
    int i;
    for (i = 0; i < attr_count; ++i)
    {
        if (is_attribute_read_only(attr_list[i].id))
        {
            return 1;
        }
    }
    
    return 0;
}

int is_object_valid(crud_object_id_t object_id)
{
    return 1;
}

attr_t* get_sdk_attr_list(crud_attribute_t* attr_list, const uint32_t attr_count)
{
    attr_t* buffer;
    buffer = malloc(sizeof(attr_t) * attr_count);
    if (buffer)
    {
        uint32_t i;
        for (i = 0; i < attr_count; ++i)
        {
            buffer[i] = crud_to_sdk_attr(attr_list[i]);
        }
        return buffer;
    }
    return 0;
}

void get_crud_attr_list(crud_attribute_t* crud_attr_list, attr_t* attr_list, const uint32_t attr_count)
{
    crud_attribute_t* buffer;
    buffer = crud_attr_list;
    if (buffer)
    {
        uint32_t i;
        for (i = 0; i < attr_count; ++i)
        {
            buffer[i] = sdk_to_crud_attr(attr_list[i]);
        }
    }
}

crud_object_id_t generate_object_id(const crud_object_type_t object_type, const uint32_t sdk_object_id)
{
    printf("generate_object_id: object type: %d\n", object_type);
    printf("generate_object_id: sdk object id: %d\n", sdk_object_id);
    return (object_type << 16 | sdk_object_id);
}

int is_switch_attribute(const crud_attr_id_t attr_id)
{
    switch (attr_id)
    {
        case CRUD_SWITCH_ATTR_NAME:
        case CRUD_SWITCH_ATTR_HASH_SEED:
        case CRUD_SWITCH_ATTR_SPLIT_MODE:
        case CRUD_SWITCH_ATTR_MAX_PORTS:
            return 1;

        default:
            return 0;
    }
}

int is_port_attribute(const crud_attr_id_t attr_id)
{
    switch (attr_id)
    {
        case CRUD_PORT_ATTR_STATE:
        case CRUD_PORT_ATTR_SPEED:
        case CRUD_PORT_ATTR_IPV4:
        case CRUD_PORT_ATTR_MTU:
            return 1;
        
        default:
            return 0;
    }
}

int is_attribute_type_present(crud_attribute_t* attr_list, const uint32_t attr_count, int(*predicate)(crud_attr_id_t))
{
    int i;
    for (i = 0; i < attr_count; ++i)
    {
        if (predicate(attr_list[i].id) == 1)
        {
            return 1;
        }
    }
    
    return 0;
}