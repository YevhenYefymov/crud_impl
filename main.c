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
    
    /******************* TEST CREATE *******************/
    crud_object_id_t object_id;
    crud_status_t create_status = create_object(attributes, attr_count, &object_id);
    assert(create_status == CRUD_STATUS_SUCCESS);

    /*******************  TEST READ  *******************/
    crud_attribute_t read_attributes[attr_count];
    crud_status_t read_status = read_object(&object_id, read_attributes, attr_count);
    assert(read_status == CRUD_STATUS_SUCCESS);
    for (i = 0; i < attr_count; ++i)
    {
        assert(attributes[i].id == read_attributes[i].id);
    }

    /******************* TEST UPDATE *******************/
    crud_attribute_t new_attributes[attr_count];
    for (i = 0; i < attr_count; ++i)
    {
        new_attributes[i].id = attributes[i].id;
        new_attributes[i].value.u32 = i*10;
    }

    crud_status_t update_status = update_object(&object_id, new_attributes, attr_count);
    assert(update_status == CRUD_STATUS_SUCCESS);

    // read the object and verify that attributes are assigned
    read_status = read_object(&object_id, read_attributes, attr_count);
    assert(read_status == CRUD_STATUS_SUCCESS);
    for (i = 0; i < attr_count; ++i)
    {
        // IMPORTANT: values of attributes are currently hard coded, so this test can pass
        // it certainly will fail some day

        assert(new_attributes[i].id == read_attributes[i].id);
        assert(new_attributes[i].value.u32 == read_attributes[i].value.u32);
    }


    /******************* TEST DELETE *******************/
    crud_status_t delete_status = delete_object(&object_id);
    assert(delete_status == CRUD_STATUS_SUCCESS);
    
    read_status = read_object(&object_id, read_attributes, attr_count);
    assert(read_status == CRUD_STATUS_FAILURE);

    update_status = update_object(&object_id, new_attributes, attr_count);
    assert(update_status == CRUD_STATUS_FAILURE);

    return 0;
}