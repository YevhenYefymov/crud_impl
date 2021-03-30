#ifndef SDK_TYPES_H
#define SDK_TYPES_H

#include <stdint.h>

typedef enum
{
    RSLT_SUCCESS,
    RSLT_FAILURE
} operation_result_t;

typedef enum
{
    SDK_OBJ_SWITCH,
    SDK_OBJ_PORT,
    SDK_OBJ_INVALID = -1
} object_type_t;

typedef enum
{
    // Switch attributes 
    SDK_OBJ_SWITCH_ATTR_NAME,
    SDK_OBJ_SWITCH_ATTR_HASH_SEED, 
    SDK_OBJ_SWITCH_ATTR_SPLIT_MODE,
    SDK_OBJ_SWITCH_ATTR_MAX_PORTS,

    // port attributes 
    SDK_OBJ_PORT_ATTR_STATE,        
    SDK_OBJ_PORT_ATTR_SPEED,        

    SDK_OBJ_PORT_ATTR_IPV4,       

    SDK_OBJ_PORT_ATTR_MTU,       

    // invalid
    SDK_OBJ_ATTR_INVALID  
} attr_type_t;

typedef union attribute_value_t
{
    int booldata;
    char chardata[32];
    uint8_t u8;
    uint32_t u32;
    uint32_t ip4;
} attr_value_t; 

typedef struct
{
    attr_type_t id;
    attr_value_t value;
} attr_t; 

#endif // SDK_TYPES_H