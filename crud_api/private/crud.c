#include "../crud.h"
#include "../sdk/sdk_object_list.h"
#include "../sdk/sdk_api.h"
#include "types_converter.h"
#include "crud_helper.h"

crud_status_t create_switch_object(crud_attribute_t* attr_list, uint32_t attr_count, crud_object_id_t *object_id)
{
    if (object_id == 0)
    {
        printf("object_id is 0\n");
        return CRUD_STATUS_FAILURE;
    }
    if (is_attribute_list_valid(attr_list, attr_count) == 0)
    {
        printf("create_object: at least one attribute in the attribute list is invalid\n");
        *object_id = 0;
        return CRUD_STATUS_ATTR_INVALID;
    }
    if (is_attribute_type_present(attr_list, attr_count, is_port_attribute))
    {
        *object_id = 0;
        printf("create_switch_object: port attributes are present in the attributes list\n");
        return CRUD_STATUS_FAILURE;
    }

    attr_t* sdk_attr_list = get_sdk_attr_list(attr_list, attr_count);
    const uint32_t sdk_object_id = sdk_create_object(crud_to_sdk_object_type(CRUD_OBJECT_TYPE_SWITCH), sdk_attr_list, attr_count);

    if (sdk_object_id == 0) // TODO: return operation status instead of object id
    {
        return CRUD_STATUS_FAILURE;
    }

    *object_id = generate_object_id(CRUD_OBJECT_TYPE_SWITCH, sdk_object_id);

    return CRUD_STATUS_SUCCESS;
}

crud_status_t read_switch_object(crud_object_id_t *object_id, crud_attribute_t* attr_list, uint32_t attr_count)
{
    if (0 == object_id)
    {
        printf("read_switch_object: object_id is 0\n");
        return CRUD_STATUS_FAILURE;
    }
    if (0 == attr_list)
    {
        printf("read_switch_object: attribute list is 0\n");
        return CRUD_STATUS_FAILURE;
    }
    if (0 == attr_count)
    {
        printf("can't read zero attributes\n");
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

    attr_t* sdk_attributes = get_sdk_attr_list(attr_list, attr_count);
    operation_result_t sdk_read_result = sdk_read_object(crud_to_sdk_object_type(CRUD_OBJECT_TYPE_SWITCH), id, sdk_attributes, attr_count);
    
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
    if (is_attribute_type_present(attr_list, attr_count, is_port_attribute))
    {
        printf("update_switch_object: port attributes are present in the attributes list\n");
        return CRUD_STATUS_FAILURE;
    }
    if (is_attribute_list_valid(attr_list, attr_count) == 0)
    {
        printf("update_switch_object: at least one attribute in the attribute list is invalid\n");
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

    // check if any read only attribute was requested for update
    if (is_read_only_attribute_present(attr_list, attr_count))
    {
        printf("update_switch_object: trying to update read only attribute\n");
        return CRUD_READ_ONLY;
    }
    
    sdk_attributes = get_sdk_attr_list(attr_list, attr_count);
    // then the update might be performed
    operation_result_t sdk_update_result = sdk_update_object(crud_to_sdk_object_type(CRUD_OBJECT_TYPE_SWITCH), id, sdk_attributes, attr_count);

    if (sdk_update_result == RSLT_FAILURE)
    {
        printf("update_switch_object: failed to update object\n");
        return CRUD_STATUS_FAILURE;
    }

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

    operation_result_t sdk_delete_result =  sdk_delete_object(crud_to_sdk_object_type(CRUD_OBJECT_TYPE_SWITCH), id);

    if (sdk_delete_result == RSLT_FAILURE)
    {
        printf("delete_switch_object: failed to delete object\n");
        return CRUD_STATUS_FAILURE;
    }

    return CRUD_STATUS_SUCCESS;
}