#ifndef CRUD_TYPES_H
#define CRUD_TYPES_H

#include <stdint.h>

typedef enum
{
    true, false
} _bool;
#define bool _bool

typedef enum 
{
    CRUD_STATUS_SUCCESS         = 0,
    CRUD_STATUS_FAILURE         = 1,
    CRUD_READ_ONLY              = 2,
    CRUD_INVALID_PARAM          = 3,
    CRUD_STATUS_ATTR_INVALID    = 4
    // add your own codes if needed 
} crud_status_t;  

typedef enum
{
    CRUD_OBJECT_TYPE_INVALID = 0,
    CRUD_OBJECT_TYPE_SWITCH  = 1,
    CRUD_OBJECT_TYPE_PORT    = 2,
} crud_object_type_t; 

typedef enum 
{   
    // Switch attributes 
    CRUD_SWITCH_ATTR_NAME,       // chardata,        read-write
    CRUD_SWITCH_ATTR_HASH_SEED,  // uint32 [0..max], read-write
    CRUD_SWITCH_ATTR_SPLIT_MODE, // uint32 [0..3],   read-only 
    CRUD_SWITCH_ATTR_MAX_PORTS,  // uint32 [1..32],  read-only  

    // port attributes 
    CRUD_PORT_ATTR_STATE,        // bool, read-write 
    CRUD_PORT_ATTR_SPEED,        // uint32 [10, 100, 1000], read-write 
                                 // but write is allowed only if STATE 
                                 // is true 

    CRUD_PORT_ATTR_IPV4,         // ip4_t, read-write any non-multicast  
                                 // and non-broadcast 

    CRUD_PORT_ATTR_MTU,          // uint32, [64..9K], read-only

    // invalid attribute value 
    CRUD_OBJECT_ATTR_INVALID = -1
} crud_attr_id_t;
 
typedef uint32_t ip4_t; 
typedef uint32_t crud_object_id_t; 

typedef union _sai_attribute_value_t
{
    bool booldata;
    char chardata[32];
    uint8_t u8;
    uint32_t u32;

    ip4_t ip4;
} crud_attr_value_t; 

typedef struct
{
    crud_attr_id_t id;
    crud_attr_value_t value;
} crud_attribute_t; 

#endif // CRUD_TYPES_H