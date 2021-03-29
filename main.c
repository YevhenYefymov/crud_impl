#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "crud_api/crud.h"

int main(int argc, char** argv)
{
    crud_attribute_t attributes[2];
    uint32_t attr_count = sizeof(attributes) / sizeof(attributes[0]);
    
    int i;
    attributes[0].id = CRUD_SWITCH_ATTR_HASH_SEED;
    attributes[0].value.u32 = 1;
    attributes[1].id = CRUD_SWITCH_ATTR_MAX_PORTS;
    attributes[0].value.u32 = 30;
    
    crud_object_id_t object_id;
    crud_status_t create_status = create_object(attributes, attr_count, &object_id);
    assert(create_status == CRUD_STATUS_SUCCESS);

    crud_attribute_t* read_attributes;
    crud_status_t read_status = read_object(&object_id, read_attributes, attr_count);
    assert(read_status == CRUD_STATUS_SUCCESS);
    for (i = 0; i < attr_count; ++i)
    {
        printf("i = %d\n", i);
        printf("my_attr = %d\tread_attr = %d\n", attributes[i].id, read_attributes[i].id);
        //assert(attributes[i].id == read_attributes[i].id);
    }

    return 0;
}