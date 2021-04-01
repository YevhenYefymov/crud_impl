#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "crud_api/crud.h"

int main(int argc, char** argv)
{
    crud_attribute_t attributes[2];
    uint32_t attr_count = sizeof(attributes) / sizeof(attributes[0]);
    
    int i;
    for (i = 0; i < attr_count; ++i)
    {
        attributes[i].id = i;
        attributes[i].value.u32 = i;
    }
    attributes[0].id = CRUD_SWITCH_ATTR_NAME;
    strncpy(attributes[0].value.chardata, "SWITCH_1", 32);

    attributes[1].id = CRUD_SWITCH_ATTR_HASH_SEED;
    attributes[1].value.u32 = 10;
    
    /******************* TEST CREATE *******************/
    printf("\n\nTEST CREATE\n");
    crud_object_id_t object_id;
    crud_status_t create_status = create_switch_object(attributes, attr_count, &object_id);
    assert(create_status == CRUD_STATUS_SUCCESS);
    printf("TEST CREATE OK\n");

    /*******************  TEST READ  *******************/
    printf("\n\nTEST READ\n");
    crud_attribute_t read_attributes[attr_count];
    crud_status_t read_status = read_switch_object(&object_id, read_attributes, attr_count);
    assert(read_status == CRUD_STATUS_SUCCESS);
    for (i = 0; i < attr_count; ++i)
    {
        assert(attributes[i].id == read_attributes[i].id);
    }
    assert(strcmp(attributes[0].value.chardata, read_attributes[0].value.chardata) == 0);
    assert(attributes[1].value.u32 == attributes[1].value.u32);
    printf("TEST READ OK\n");


    /******************* TEST UPDATE *******************/
    printf("\n\nTEST UPDATE\n");
    // update only the name, leave hash seed as it was
    crud_attribute_t new_attributes[1];
    new_attributes[0].id = attributes[0].id;
    strncpy(attributes[0].value.chardata, "NEW_SWITCH_NAME", 32);

    crud_status_t update_status = update_switch_object(&object_id, new_attributes, 1);
    assert(update_status == CRUD_STATUS_SUCCESS);

    // read the object and verify that only the requested attributes were updated
    read_status = read_switch_object(&object_id, read_attributes, attr_count);
    assert(read_status == CRUD_STATUS_SUCCESS);
    
    assert(strcmp(new_attributes[0].value.chardata, read_attributes[0].value.chardata) == 0);
    assert(attributes[1].value.u32 == read_attributes[1].value.u32);
    printf("TEST UPDATE OK\n");


    /******************* TEST DELETE *******************/
    printf("\n\nTEST DELETE\n");
    crud_status_t delete_status = delete_switch_object(&object_id);
    assert(delete_status == CRUD_STATUS_SUCCESS);
    
    read_status = read_switch_object(&object_id, read_attributes, attr_count);
    assert(read_status == CRUD_STATUS_FAILURE);

    update_status = update_switch_object(&object_id, new_attributes, attr_count);
    assert(update_status == CRUD_STATUS_FAILURE);
    printf("TEST DELETE OK\n");


    /******************* TEST PORT ATTRIBUTES *******************/
    printf("\n\nTEST PORT_ATTRIBUTES\n");
    crud_attribute_t port_attrs[2];
    port_attrs[0].id = CRUD_PORT_ATTR_IPV4;
    port_attrs[1].id = CRUD_PORT_ATTR_MTU;
    create_status = create_switch_object(port_attrs, 2, &object_id);
    assert(create_status == CRUD_STATUS_FAILURE);
    printf("TEST PORT ATTRIBUTES OK\n");

    /******************* TEST UPDATE READ ONLY *******************/
    printf("\n\nTEST UPDATE READ ONLY\n");
    crud_attribute_t read_only_attrs[2];

    read_only_attrs[0].id = CRUD_SWITCH_ATTR_NAME; // this one's not read only
    strncpy(read_only_attrs[0].value.chardata, "MY_SWITCH", 32);
    read_only_attrs[1].id = CRUD_SWITCH_ATTR_MAX_PORTS; // this one is
    read_only_attrs[1].value.u32 = 10;

    // create object
    crud_object_id_t read_only_object;
    crud_status_t read_only_create_status = create_switch_object(read_only_attrs, 2, &read_only_object);
    assert(read_only_create_status == CRUD_STATUS_SUCCESS);

    // try to update the name attribute - should succeed
    crud_attribute_t* new_name = malloc(sizeof(crud_attribute_t));
    if (new_name)
    {
        new_name->id = CRUD_SWITCH_ATTR_NAME;
        strncpy(new_name->value.chardata, "MY_SWITCH", 32);
    }
    crud_status_t update_name_stats = update_switch_object(&read_only_object, new_name, 1);
    assert(update_name_stats == CRUD_STATUS_SUCCESS);

    // now try to update max ports value - this should fail
    crud_attribute_t* new_max_ports = malloc(sizeof(crud_attribute_t));
    if (new_max_ports)
    {
        new_max_ports->id = CRUD_SWITCH_ATTR_MAX_PORTS;
        new_max_ports->value.u32 = 20;
    }
    crud_status_t update_max_ports_status = update_switch_object(&read_only_object, new_max_ports, 1);
    assert(update_max_ports_status == CRUD_READ_ONLY);

    printf("TEST UPDATE READ ONLY OK\n");
    return 0;
}