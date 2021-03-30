#ifndef TYPES_CONVERTER_H
#define TYPES_CONVERTER_H

#include "crud_types.h"
#include "../sdk/sdk_types.h"

/*********************************************************/
/***********************CRUD TO SDK***********************/
/*********************************************************/

object_type_t crud_to_sdk_object_type(const crud_object_type_t in)
{
    switch (in)
    {
        case CRUD_OBJECT_TYPE_SWITCH:   return SDK_OBJ_SWITCH;
        case CRUD_OBJECT_TYPE_PORT:     return SDK_OBJ_PORT;

        default:
            return SDK_OBJ_INVALID;
    }
}

attr_type_t crud_to_sdk_attr_type(const crud_attr_id_t in)
{
    switch (in)
    {
        case CRUD_SWITCH_ATTR_NAME:         return SDK_OBJ_SWITCH_ATTR_NAME;
        case CRUD_SWITCH_ATTR_HASH_SEED:    return SDK_OBJ_SWITCH_ATTR_HASH_SEED;
        case CRUD_SWITCH_ATTR_SPLIT_MODE:   return SDK_OBJ_SWITCH_ATTR_SPLIT_MODE;
        case CRUD_SWITCH_ATTR_MAX_PORTS:    return SDK_OBJ_SWITCH_ATTR_MAX_PORTS;
        case CRUD_PORT_ATTR_STATE:          return SDK_OBJ_PORT_ATTR_STATE;
        case CRUD_PORT_ATTR_SPEED:          return SDK_OBJ_PORT_ATTR_SPEED;
        case CRUD_PORT_ATTR_IPV4:           return SDK_OBJ_PORT_ATTR_IPV4;
        case CRUD_PORT_ATTR_MTU:            return SDK_OBJ_PORT_ATTR_MTU;

        default:
            return SDK_OBJ_ATTR_INVALID;
    }
}

attr_t crud_to_sdk_attr(const crud_attribute_t in)
{
    attr_t out;
    
    // TODO: need to convert union somehow...

    out.id = crud_to_sdk_attr_type(in.id);
    out.value.u32 = 1;

    return out;
}


/*********************************************************/
/***********************SDK TO CRUD***********************/
/*********************************************************/
crud_attr_id_t sdk_to_crud_attr_type(const attr_type_t in)
{
    switch (in)
    {
        case SDK_OBJ_SWITCH_ATTR_NAME:          return  CRUD_SWITCH_ATTR_NAME;
        case SDK_OBJ_SWITCH_ATTR_HASH_SEED:     return  CRUD_SWITCH_ATTR_HASH_SEED;
        case SDK_OBJ_SWITCH_ATTR_SPLIT_MODE:    return  CRUD_SWITCH_ATTR_SPLIT_MODE;
        case SDK_OBJ_SWITCH_ATTR_MAX_PORTS:     return  CRUD_SWITCH_ATTR_MAX_PORTS;
        case SDK_OBJ_PORT_ATTR_STATE:           return  CRUD_PORT_ATTR_STATE;
        case SDK_OBJ_PORT_ATTR_SPEED:           return  CRUD_PORT_ATTR_SPEED;      
        case SDK_OBJ_PORT_ATTR_IPV4:            return  CRUD_PORT_ATTR_IPV4;     
        case SDK_OBJ_PORT_ATTR_MTU:             return  CRUD_PORT_ATTR_MTU;      

        default:
            return CRUD_OBJECT_ATTR_INVALID;
    }
}

crud_attribute_t sdk_to_crud_attr(const attr_t  in)
{
    crud_attribute_t out;
    
    // TODO: need to convert union somehow...

    out.id = sdk_to_crud_attr_type(in.id);
    out.value.u32 = 1;

    return out;
}

#endif // TYPES_CONVERTER_H