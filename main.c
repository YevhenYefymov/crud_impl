#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "crud_api/crud.h"

// test create_switch_object function
// a new switch object should be created and the positive status of
// the operation should be returned
void crud_test_create()
{
    printf("\n\nTEST CREATE\n");
    crud_attribute_t attributes[2];
    uint32_t attr_count = sizeof(attributes) / sizeof(attributes[0]);
    
    attributes[0].id = CRUD_SWITCH_ATTR_NAME;
    strncpy(attributes[0].value.chardata, "SWITCH_1", 32);

    attributes[1].id = CRUD_SWITCH_ATTR_HASH_SEED;
    attributes[1].value.u32 = 10;
        
    crud_object_id_t object_id;
    crud_status_t create_status = create_switch_object(attributes, attr_count, &object_id);
    assert(create_status == CRUD_STATUS_SUCCESS);
    printf("TEST CREATE OK\n");
}

// test read_switch_object function
// should verify that read attribute list matches the one that
// was passed during the creation of the object
void crud_test_read()
{
    printf("\n\nTEST READ\n");
    
    // create the object first
    crud_attribute_t attributes[2];
    uint32_t attr_count = sizeof(attributes) / sizeof(attributes[0]);
    
    attributes[0].id = CRUD_SWITCH_ATTR_NAME;
    strncpy(attributes[0].value.chardata, "SWITCH_1", 32);
    
    attributes[1].id = CRUD_SWITCH_ATTR_HASH_SEED;
    attributes[1].value.u32 = 10;
            
    crud_object_id_t object_id;
    crud_status_t create_status = create_switch_object(attributes, attr_count, &object_id);
    assert(create_status == CRUD_STATUS_SUCCESS);

    // then read the object and verify that values match
    crud_attribute_t read_attributes[attr_count];
    crud_status_t read_status = read_switch_object(&object_id, read_attributes, attr_count);
    assert(read_status == CRUD_STATUS_SUCCESS);
    int i;
    for (i = 0; i < attr_count; ++i)
    {
        assert(attributes[i].id == read_attributes[i].id);
    }
    assert(strcmp(attributes[0].value.chardata, read_attributes[0].value.chardata) == 0);
    assert(attributes[1].value.u32 == attributes[1].value.u32);
    printf("TEST READ OK\n");
}

// test update_switch_object function
// should assign new values to the attributes of the object,
// read the object after update and verify that
// attributes were changed
void crud_test_update()
{
    printf("\n\nTEST UPDATE\n");
    
    // create the object first
    crud_attribute_t attributes[2];
    uint32_t attr_count = sizeof(attributes) / sizeof(attributes[0]);
    
    attributes[0].id = CRUD_SWITCH_ATTR_NAME;
    strncpy(attributes[0].value.chardata, "SWITCH_1", 32);
    
    attributes[1].id = CRUD_SWITCH_ATTR_HASH_SEED;
    attributes[1].value.u32 = 10;
            
    crud_object_id_t object_id;
    crud_status_t create_status = create_switch_object(attributes, attr_count, &object_id);
    assert(create_status == CRUD_STATUS_SUCCESS);

    // update only the name, leave hash seed as it was
    const uint32_t changed_attrs_count = 1;
    crud_attribute_t new_attributes[changed_attrs_count];
    new_attributes[0].id = attributes[0].id;
    strncpy(attributes[0].value.chardata, "NEW_SWITCH_NAME", 32);

    crud_status_t update_status = update_switch_object(&object_id, new_attributes, changed_attrs_count);
    assert(update_status == CRUD_STATUS_SUCCESS);

    // read the object and verify that only the requested attributes were updated
    crud_attribute_t read_attributes[attr_count];
    crud_status_t read_status = read_switch_object(&object_id, read_attributes, attr_count);
    assert(read_status == CRUD_STATUS_SUCCESS);
    
    assert(strcmp(new_attributes[0].value.chardata, read_attributes[0].value.chardata) == 0);
    assert(attributes[1].value.u32 == read_attributes[1].value.u32);
    printf("TEST UPDATE OK\n");
}

// test delete_switch_object function
// should delete object with given id and verify
// that this object can't be read or updated
void crud_test_delete()
{
    printf("\n\nTEST DELETE\n");

    // create the object first
    crud_attribute_t attributes[2];
    uint32_t attr_count = sizeof(attributes) / sizeof(attributes[0]);
    
    attributes[0].id = CRUD_SWITCH_ATTR_NAME;
    strncpy(attributes[0].value.chardata, "SWITCH_1", 32);
    
    attributes[1].id = CRUD_SWITCH_ATTR_HASH_SEED;
    attributes[1].value.u32 = 10;
            
    crud_object_id_t object_id;
    crud_status_t create_status = create_switch_object(attributes, attr_count, &object_id);
    assert(create_status == CRUD_STATUS_SUCCESS);

    // delete the object
    crud_status_t delete_status = delete_switch_object(&object_id);
    assert(delete_status == CRUD_STATUS_SUCCESS);
    
    // try to read deleted object - should fail
    crud_attribute_t read_attributes[attr_count];
    crud_status_t read_status = read_switch_object(&object_id, read_attributes, attr_count);
    assert(read_status == CRUD_STATUS_FAILURE);

    // try to update deleted object - should fail as well
    const uint32_t changed_attrs_count = 1;
    crud_attribute_t new_attributes[changed_attrs_count];
    new_attributes[0].id = attributes[0].id;
    strncpy(attributes[0].value.chardata, "NEW_SWITCH_NAME", 32);
    crud_status_t update_status = update_switch_object(&object_id, new_attributes, 1);
    assert(update_status == CRUD_STATUS_FAILURE);
    printf("TEST DELETE OK\n");
}

// test calling create_switch_object with port attributes
// an attempt to create switch object with port (or any other non-switch)
// attributes should fail
void crud_test_create_switch_with_port_attributes()
{
    printf("\n\nTEST CREATE SWITCH WITH PORT_ATTRIBUTES\n");
    
    // create list of port attributes
    crud_attribute_t port_attrs[2];
    uint32_t attr_count = sizeof(port_attrs) / sizeof(port_attrs[0]);
    port_attrs[0].id = CRUD_PORT_ATTR_IPV4;
    port_attrs[1].id = CRUD_PORT_ATTR_MTU;

    // then try to create switch object with such list - this should fail
    crud_object_id_t object_id;
    crud_status_t create_status = create_switch_object(port_attrs, attr_count, &object_id);
    assert(create_status == CRUD_STATUS_FAILURE);
    printf("TEST CREATE SWITCH WITH PORT_ATTRIBUTES OK\n");
}

// test calling update_switch_object with a read only attribute
// an attempt to modify a read only attribute of an object should fail
void crud_test_update_read_only_attributes()
{
    printf("\n\nTEST UPDATE READ ONLY ATTRIBUTES\n");
    crud_attribute_t read_only_attrs[2];

    read_only_attrs[0].id = CRUD_SWITCH_ATTR_NAME; // this one's not read only
    strncpy(read_only_attrs[0].value.chardata, "MY_SWITCH", 32);
    read_only_attrs[1].id = CRUD_SWITCH_ATTR_MAX_PORTS; // this one is
    read_only_attrs[1].value.u32 = 10;

    // create object
    crud_object_id_t object_id;
    crud_status_t read_only_create_status = create_switch_object(read_only_attrs, 2, &object_id);
    assert(read_only_create_status == CRUD_STATUS_SUCCESS);

    // try to update the name attribute - should succeed
    crud_attribute_t new_name[1];
    new_name[0].id = CRUD_SWITCH_ATTR_NAME;
    strncpy(new_name[0].value.chardata, "MY_SWITCH", 32);

    crud_status_t update_name_status = update_switch_object(&object_id, new_name, 1);
    assert(update_name_status == CRUD_STATUS_SUCCESS);

    // now try to update max ports value - this should fail
    crud_attribute_t new_max_ports[1];
    new_max_ports[0].id = CRUD_SWITCH_ATTR_MAX_PORTS;
    new_max_ports[0].value.u32 = 20;

    crud_status_t update_max_ports_status = update_switch_object(&object_id, new_max_ports, 1);
    assert(update_max_ports_status == CRUD_READ_ONLY);

    printf("TEST UPDATE READ ONLY ATTRIBUTES OK\n");
}

int main(int argc, char** argv)
{
    crud_test_create();
    crud_test_read();
    crud_test_update();
    crud_test_delete();
    crud_test_create_switch_with_port_attributes();
    crud_test_update_read_only_attributes();

    return 0;
}