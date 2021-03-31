#ifndef CRUD_HELPER_H
#define CRUD_HELPER_H

#include "crud_types.h"
#include "../sdk/sdk_types.h"

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

// check if the attribute is of the port object
// return 1 if it is, 0 otherwise
int is_port_attribute(const crud_attr_id_t attr_id);

// check if attributes of certain object are present in the list
// takes corresponding predicate as a parameter (e.g. is_switch_attribute)
// return 1 if at least one such attribute is found, 0 otherwise
int is_attribute_type_present(crud_attribute_t* attr_list, const uint32_t attr_count, int(*predicate)(crud_attr_id_t));

// generate new sdk attribute list from given crud attribute list
// return the newly created sdk attribute list
attr_t* get_sdk_attr_list(crud_attribute_t* attr_list, const uint32_t attr_count);

// get crud attribute list from sdk attribute list
// takes crud attribute list to fill as input
void get_crud_attr_list(crud_attribute_t* crud_attr_list, attr_t* attr_list, const uint32_t attr_count);

// generate crud object id based on the type of the object (switch, port etc) and the id returned from sdk
// return generated object id
crud_object_id_t generate_object_id(const crud_object_type_t object_type, const uint32_t sdk_object_id);

#endif // CRUD_HELPER_H