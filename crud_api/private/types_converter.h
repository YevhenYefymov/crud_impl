#ifndef TYPES_CONVERTER_H
#define TYPES_CONVERTER_H

#include <string.h>

#include "../crud_types.h"
#include "../../sdk/sdk_types.h"

/*********************************************************/
/***********************CRUD TO SDK***********************/
/*********************************************************/

object_type_t crud_to_sdk_object_type(const crud_object_type_t in);
attr_type_t crud_to_sdk_attr_type(const crud_attr_id_t in);
void set_sdk_attr_value(const crud_attribute_t in, attr_t* out);
attr_t crud_to_sdk_attr(const crud_attribute_t in);

/*********************************************************/
/***********************SDK TO CRUD***********************/
/*********************************************************/
crud_object_type_t sdk_to_crud_object_type(const object_type_t in);
crud_attr_id_t sdk_to_crud_attr_type(const attr_type_t in);
void set_crud_attr_value(const attr_t in, crud_attribute_t* out);
crud_attribute_t sdk_to_crud_attr(const attr_t  in);

#endif // TYPES_CONVERTER_H