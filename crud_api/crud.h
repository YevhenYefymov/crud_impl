#ifndef CRUD_H
#define CRUD_H

#include "crud_types.h"

//create_object
//input parameters: array of crud_attribute_t, size of array 
//output parameters: crud_object_id_t 
//should do: create object (in the internal array), generate OID as an output, return error if input is invalid 
crud_status_t create_object(crud_attribute_t* attr_list, uint32_t attr_count, crud_object_id_t *object_id);

//read_object 
//input parameters: object ID, array of crud_attribute_t, size of array 
//output parameters: values of given attributes 
crud_status_t read_object(crud_object_id_t *object_id, crud_attribute_t* attr_list, uint32_t attr_count);

//update_object 
//input parameters: object ID, single crud_attribute_t 
//output parameters: none 
crud_status_t update_object(crud_object_id_t *object_id, crud_attribute_t* attr_list, uint32_t attr_count);

//delete_object 
//input parameters: object ID  
//output parameters: none 
crud_status_t delete_object(crud_object_id_t *object_id);

#endif // CRUD_H