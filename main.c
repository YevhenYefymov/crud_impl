#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "crud_api/crud.h"

int main(int argc, char** argv)
{
    crud_attribute_t attributes[4];
    uint32_t attr_count = sizeof(attributes) / sizeof(attributes[0]);
    
    int i;
    for (i = 0; i < attr_count; ++i)
    {
        attributes[i].id = i;
    }
    
    crud_object_id_t object_id;
    crud_status_t create_status = create_object(attributes, attr_count, &object_id);
    assert(create_status == CRUD_STATUS_SUCCESS);

    crud_attribute_t read_attributes[attr_count];
    crud_status_t read_status = read_object(&object_id, read_attributes, attr_count);
    assert(read_status == CRUD_STATUS_SUCCESS);
    for (i = 0; i < attr_count; ++i)
    {
        assert(attributes[i].id == read_attributes[i].id);
    }

    return 0;
}