#include "crud.h"
#include "../sdk/sdk_object_list.h"
#include "../sdk/sdk_api.h"
#include "types_converter.h"

/***************** HELPER FUNCTIONS DECLARATION *****************/
// validate attribute according to its limitations,
// described at the attribute definition
// return 1 if the attribute is valid, 0 otherwise
int is_attribute_valid(const crud_attribute_t attr);

// validate attribute list
// return 1 if all attributes are valid, 0 if at least one attribute is invalid
int is_attribute_list_valid(crud_attribute_t* attr_list, const uint32_t attr_count);

// check if attribute is read only
// read only attributes are marked at the definition
// return 1 if the attribute is read only, 0 otherwise
int is_attribute_read_only(const crud_attr_id_t attr_id);

// special case for CRUD_PORT_ATTR_SPEED, since it can be writable only in
// certain condition (CRUD_PORT_ATTR_STATE is true)
// return 1 if CRUD_PORT_ATTR_SPEED is read only, 0 otherwise
int is_port_speed_read_only(const crud_attribute_t port_state_attr);

// check if any read only attribute is present in the attribute list
// return 1 if at leas one attribute is read_only, 0 otherwise
int is_read_only_attribute_present(crud_attribute_t* attr_list, const uint32_t attr_count);

// validate object id
// return 1 if the object is valid, 0 if the object is invalid or deleted
int is_object_valid(crud_object_id_t object_id);

// check if the attribute is of the switch object
// return 1 if it is, 0 otherwise
int is_switch_attribute(const crud_attr_id_t attr_id);

// check if attributes related to switch object are present int the attribute list
// return 1 if at least one such attribute is found, 0 otherwise
int is_switch_attributes_present(crud_attribute_t* attr_list, const uint32_t attr_count);

// check if the attribute is of the port object
// return 1 if it is, 0 otherwise
int is_port_attribute(const crud_attr_id_t attr_id);

// check if attributes related to port object are present int the attribute list
// return 1 if at least one such attribute is found, 0 otherwise
int is_port_attributes_present(crud_attribute_t* attr_list, const uint32_t attr_count);

attr_t* get_sdk_attr_list(crud_attribute_t* attr_list, const uint32_t attr_count);

void get_crud_attr_list(crud_attribute_t* crud_attr_list, attr_t* attr_list, const uint32_t attr_count);

crud_object_type_t get_object_type(crud_attribute_t* attr_list, const uint32_t attr_count);

// generate crud object id based on the type of the object (switch, port etc) and the id returned from sdk
// return generated object id
crud_object_id_t generate_object_id(const crud_object_type_t object_type, const uint32_t sdk_object_id);


/***************** INTERFACE IMPLEMENTATION *****************/
crud_status_t create_switch_object(crud_attribute_t* attr_list, uint32_t attr_count, crud_object_id_t *object_id)
{
    if (is_port_attributes_present(attr_list, attr_count))
    {
        printf("create_switch_object: port attributes are present in the attributes list\n");
        return CRUD_STATUS_FAILURE;
    }
    if (is_attribute_list_valid(attr_list, attr_count) == 0)
    {
        printf("create_object: at leas one attribute in the attribute list is invalid\n");
        return CRUD_STATUS_ATTR_INVALID;
    }
    if (object_id == 0)
    {
        printf("object_id is 0\n");
        return CRUD_STATUS_FAILURE;
    }

    attr_t* sdk_attr_list = get_sdk_attr_list(attr_list, attr_count);
    uint32_t sdk_object_id = sdk_create_object(crud_to_sdk_object_type(CRUD_OBJECT_TYPE_SWITCH), sdk_attr_list);
    *object_id = generate_object_id(CRUD_OBJECT_TYPE_SWITCH, sdk_object_id);

    return CRUD_STATUS_SUCCESS;
}

crud_status_t read_switch_object(crud_object_id_t *object_id, crud_attribute_t* attr_list, uint32_t attr_count)
{
    if (object_id == 0)
    {
        printf("read_switch_object: object_id is 0");
        return CRUD_STATUS_FAILURE;
    }
    if (attr_list == 0)
    {
        printf("read_switch_object: attribute list is 0\n");
        return CRUD_STATUS_FAILURE;
    }

    // crud_object_id_t represents 16 bits of object type and 16 bits of object id
    // shift object_id right by 16 bits to read object's type
    // 16 bit mask should be applied to read the id
    const uint32_t type = *object_id >> 16;
    if (type != CRUD_OBJECT_TYPE_SWITCH)
    {
        printf("read_switch_object: object type is not switch\n");
        return CRUD_STATUS_FAILURE;
    }

    const uint32_t mask = 0xFFFF;
    const uint32_t id = *object_id & mask;
    attr_t* sdk_attributes;
    operation_result_t sdk_read_result = sdk_read_object(id, &sdk_attributes);
    
    if (sdk_read_result != RSLT_SUCCESS)
    {
        printf("read_switch_object: failed to read object\n");
        return CRUD_STATUS_FAILURE;
    }

    get_crud_attr_list(attr_list, sdk_attributes, attr_count);

    return CRUD_STATUS_SUCCESS;
}

crud_status_t update_switch_object(crud_object_id_t *object_id, crud_attribute_t* attr_list, uint32_t attr_count)
{
    if (attr_list == 0)
    {
        printf("update_switch_object: attr_list is 0\n");
        return CRUD_STATUS_FAILURE;
    }
    if (is_port_attributes_present(attr_list, attr_count))
    {
        printf("create_switch_object: port attributes are present in the attributes list\n");
        return CRUD_STATUS_FAILURE;
    }
    if (is_attribute_list_valid(attr_list, attr_count) == 0)
    {
        printf("create_object: at leas one attribute in the attribute list is invalid\n");
        return CRUD_STATUS_ATTR_INVALID;
    }
    if (object_id == 0)
    {
        printf("update_switch_object: object_id is 0\n");
        return CRUD_STATUS_FAILURE;
    }

    // crud_object_id_t represents 16 bits of object type and 16 bits of object id
    // shift object_id right by 16 bits to read object's type
    // 16 bit mask should be applied to read the id
    const uint32_t type = *object_id >> 16;
    if (type != CRUD_OBJECT_TYPE_SWITCH)
    {
        printf("update_switch_object: object type is not switch\n");
        return CRUD_STATUS_FAILURE;
    }
    const int mask = 0xFFFF;
    const uint32_t id = (*object_id & mask);
    attr_t* sdk_attributes;
    printf("update_switch_object: object id: %d \n", id);

    // check if any of the current attributes is read only
    operation_result_t sdk_read_result = sdk_read_object(id, &sdk_attributes);
    if (sdk_read_result != RSLT_SUCCESS)
    {
        printf("update_switch_object: failed to read object\n");
        return CRUD_STATUS_FAILURE;
    }
    
    crud_attribute_t* current_attributes;
    current_attributes = malloc(sizeof(crud_attribute_t) * attr_count);
    get_crud_attr_list(current_attributes, sdk_attributes, attr_count);
    if (is_read_only_attribute_present(current_attributes, attr_count))
    {
        printf("update_switch_object: read only attribute is in the current list\n");
        return CRUD_READ_ONLY;
    }
    
    sdk_attributes = get_sdk_attr_list(attr_list, attr_count);
    // then the update might be performed
    operation_result_t sdk_update_result = sdk_update_object(id, sdk_attributes);

    if (sdk_update_result == RSLT_FAILURE)
    {
        printf("update_switch_object: failed to update object\n");
        return CRUD_STATUS_FAILURE;
    }

    attr_t* _sdk_attributes;
    sdk_read_object(id, &_sdk_attributes);

    return CRUD_STATUS_SUCCESS;
}

crud_status_t delete_switch_object(crud_object_id_t *object_id)
{
    if (object_id == 0)
    {
        printf("delete_switch_object: object_id is 0\n");
        return CRUD_STATUS_FAILURE;
    }

    // crud_object_id_t represents 16 bits of object type and 16 bits of object id
    // shift object_id right by 16 bits to read object's type
    // 16 bit mask should be applied to read the id
    const uint32_t type = *object_id >> 16;
    if (type != CRUD_OBJECT_TYPE_SWITCH)
    {
        printf("delete_switch_object: object type is not switch\n");
        return CRUD_STATUS_FAILURE;
    }
    const int mask = 0xFFFF;
    const uint32_t id = (*object_id & mask);

    operation_result_t sdk_delete_result =  sdk_delete_object(id);

    if (sdk_delete_result == RSLT_FAILURE)
    {
        printf("delete_switch_object: failed to delete object\n");
        return CRUD_STATUS_FAILURE;
    }

    return CRUD_STATUS_SUCCESS;
}


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
    if (is_switch_attributes_present(attr_list, attr_count) && is_port_attributes_present(attr_list, attr_count))
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

int is_switch_attributes_present(crud_attribute_t* attr_list, const uint32_t attr_count)
{
    int i;
    for (i = 0; i < attr_count; ++i)
    {
        if (is_switch_attribute(attr_list[i].id) == 1)
        {
            return 1;
        }
    }
    return 0;
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

int is_port_attributes_present(crud_attribute_t* attr_list, const uint32_t attr_count)
{
    int i;
    for (i = 0; i < attr_count; ++i)
    {
        if (is_port_attribute(attr_list[i].id) == 1)
        {
            return 1;
        }
    }
    return 0;
}