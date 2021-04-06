#ifndef SDK_TYPES_H
#define SDK_TYPES_H

#include <stdint.h>
#include <stdbool.h>

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
    SDK_OBJ_SWITCH_ATTR_NAME,           // chardata
    SDK_OBJ_SWITCH_ATTR_HASH_SEED,      // uint32 [0..max]
    SDK_OBJ_SWITCH_ATTR_SPLIT_MODE,     // uint32 [0..3]
    SDK_OBJ_SWITCH_ATTR_MAX_PORTS,      // uint32 [1..32]

    // port attributes 
    SDK_OBJ_PORT_ATTR_STATE,            // bool
    SDK_OBJ_PORT_ATTR_SPEED,            // uint32 [10, 100, 1000]
    SDK_OBJ_PORT_ATTR_IPV4,             // uint32_t
    SDK_OBJ_PORT_ATTR_MTU,              // uint32, [64..9K]

    // invalid
    SDK_OBJ_ATTR_INVALID  
} attr_type_t;

typedef union attribute_value_t
{
    bool booldata;
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

typedef enum
{
    SWITCH_MAX_COUNT = 3,
    PORT_MAX_COUNT = 32,
} object_max_count_t;

#endif // SDK_TYPES_H