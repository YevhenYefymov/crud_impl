#include "crud.h"
#include "../sdk/sdk_object_list.h"
#include "../sdk/sdk_api.h"
#include "types_converter.h"

// validate attribute according to its limitations,
// described at the attribute definition
// return 1 if the attribute is valid, 0 otherwise
int is_attribute_valid(crud_attr_id_t attr_id)
{
    return 1;
}

// validate attribute list
// return 1 if all attributes are valid, 0 if at least one attribute is invalid
int is_attribute_list_valid(crud_attribute_t* attr_list, const uint32_t attr_count)
{
    uint32_t i;
    for (i = 0; i < attr_count; ++i)
    {
        if (is_attribute_valid(attr_list[i].id) != 1)
        {
            printf("is_attribute_list_valid: at least 1 attribute is invalid\n");
            return 0;
        }
    }
    return 1;
}

// check if attribute is read only
// read only attributes are marked at the definition
// return 1 if the attribute is read only, 0 otherwise
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

// special case for CRUD_PORT_ATTR_SPEED, since it can be writable only in
// certain condition (CRUD_PORT_ATTR_STATE is true)
// return 1 if CRUD_PORT_ATTR_SPEED is read only, 0 otherwise
int is_port_speed_read_only(const crud_attribute_t port_state_attr)
{
    if (CRUD_PORT_ATTR_STATE != port_state_attr.id)
    {
        return 0;
    }
    
    return (port_state_attr.value.booldata == true);
}

// validate object id
// return 1 if the object is valid, 0 if the object is invalid or deleted
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

crud_object_type_t get_object_type(crud_attribute_t* attr_list, const uint32_t attr_count)
{
    return CRUD_OBJECT_TYPE_SWITCH;
}

crud_object_id_t generate_object_id(const crud_object_type_t object_type, const uint32_t sdk_object_id)
{
    printf("generate_object_id: object type: %d\n", object_type);
    printf("generate_object_id: sdk object id: %d\n", sdk_object_id);
    return (object_type << 16 | sdk_object_id);
}

crud_status_t create_object(crud_attribute_t* attr_list, uint32_t attr_count, crud_object_id_t* object_id)
{
    if (attr_list == 0)
    {
        printf("create_object: attr_list is 0");
        return CRUD_STATUS_FAILURE;
    }

    if (is_attribute_list_valid(attr_list, attr_count) != 1)
    {
        printf("create_object: attribute is invalid\n");
        return CRUD_STATUS_ATTR_INVALID;
    }

    if (object_id == 0)
    {
        printf("create_object: object_id is 0");
        return CRUD_STATUS_FAILURE;
    }

    attr_t* sdk_attr_list = get_sdk_attr_list(attr_list, attr_count);
    uint32_t sdk_object_id = sdk_create_object(crud_to_sdk_object_type(CRUD_OBJECT_TYPE_SWITCH), sdk_attr_list);
    *object_id = generate_object_id(get_object_type(attr_list, attr_count), sdk_object_id);    

    return CRUD_STATUS_SUCCESS;
}

crud_status_t read_object(crud_object_id_t *object_id, crud_attribute_t* attr_list, uint32_t attr_count)
{
    if (object_id == 0)
    {
        printf("read_object: object_id is 0");
        return CRUD_STATUS_FAILURE;
    }

    const uint32_t mask = 0xFFFF;
    const uint32_t id = *object_id & mask;
    attr_t* sdk_attributes;
    operation_result_t sdk_read_result = sdk_read_object(id, &sdk_attributes);
    
    if (sdk_read_result != RSLT_SUCCESS)
    {
        printf("read_object: failed to read object\n");
        return CRUD_STATUS_FAILURE;
    }

    get_crud_attr_list(attr_list, sdk_attributes, attr_count);

    return CRUD_STATUS_SUCCESS;
}

crud_status_t update_object(crud_object_id_t *object_id, crud_attribute_t* attr_list, uint32_t attr_count)
{
    if (object_id == 0)
    {
        return CRUD_STATUS_FAILURE;
    }

    const int mask = 0xFFFF;
    const uint32_t id = (*object_id & mask);
    attr_t* sdk_attributes = get_sdk_attr_list(attr_list, attr_count);

    operation_result_t sdk_update_result = sdk_update_object(id, sdk_attributes);

    if (sdk_update_result == RSLT_FAILURE)
    {
        printf("update_object: failed to update object\n");
        return CRUD_STATUS_FAILURE;
    }

    return CRUD_STATUS_SUCCESS;
}

crud_status_t delete_object(crud_object_id_t *object_id)
{
    if (object_id == 0)
    {
        printf("object_id is 0\n");
        return CRUD_STATUS_FAILURE;
    }

    const int mask = 0xFFFF;
    const uint32_t id = (*object_id & mask);

    operation_result_t sdk_delete_result =  sdk_delete_object(id);

    if (sdk_delete_result == RSLT_FAILURE)
    {
        printf("delete_object: failed to delete object\n");
        return CRUD_STATUS_FAILURE;
    }

    return CRUD_STATUS_SUCCESS;
}