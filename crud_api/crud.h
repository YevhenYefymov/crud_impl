#ifndef CRUD_H
#define CRUD_H

#include "crud_types.h"

// create_switch_object
// input parameters: array of crud_attribute_t, size of array 
// output parameters: crud_object_id_t 
// should do: create switch object (in the internal array), generate OID as an output, return error if input is invalid 
crud_status_t create_switch_object(crud_attribute_t* attr_list, uint32_t attr_count, crud_object_id_t *object_id);

// read_switch_object 
// input parameters: object ID, array of crud_attribute_t, size of array 
// output parameters: values of given attributes 
// should do: fill in value part of the attr_list. Return error if input is invalid 
crud_status_t read_switch_object(crud_object_id_t *object_id, crud_attribute_t* attr_list, uint32_t attr_count);

// update_switch_object 
// input parameters: object ID, single crud_attribute_t 
// output parameters: none 
// should do: apply values from given attributes to the internal array. On following read an updated value should be returned. Return error if input is invalid
crud_status_t update_switch_object(crud_object_id_t *object_id, crud_attribute_t* attr_list, uint32_t attr_count);

// delete_switch_object 
// input parameters: object ID  
// output parameters: none 
// should do: remove object. Following read/update should fail. 
crud_status_t delete_switch_object(crud_object_id_t *object_id);

#endif // CRUD_H