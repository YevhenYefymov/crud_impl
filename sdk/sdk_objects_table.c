#include "sdk_objects_table.h"

objects_table_t* current_objects_table;

objects_table_t* create_table()
{
    current_objects_table = malloc(sizeof(objects_table_t));
    if (current_objects_table)
    {
        current_objects_table->head = 0;
    }

    return current_objects_table;
}

operation_result_t delete_table()
{
    return RSLT_SUCCESS;
}

objects_table_entry_t* create_entry(const object_type_t key, object_list_t* objects)
{
    objects_table_entry_t* entry = malloc(sizeof(objects_table_entry_t));
    if (entry)
    {
        entry->key = key;
        entry->objects = objects;
        entry->next = 0;
    }
    
    return entry;
}

operation_result_t add_entry(const object_type_t key, object_list_t* objects)
{
    objects_table_entry_t* entry = create_entry(key, objects);
    if (entry)
    {
        entry->next = current_objects_table->head;
        current_objects_table->head = entry;
        return RSLT_SUCCESS;
    }
    
    return RSLT_FAILURE;
}

objects_table_entry_t* get_entry(const object_type_t key)
{
    objects_table_entry_t* entry = current_objects_table->head;
    while (entry != 0)
    {
        if (entry->key == key)
        {
            return entry;
        }

        entry = entry->next;
    }
}

int is_max_objects(objects_table_entry_t* list, const object_type_t key)
{
    const int list_size = list->objects->size;
    object_max_count_t max_count = 0;
    switch (key)
    {
        case SDK_OBJ_SWITCH:
            max_count = SWITCH_MAX_COUNT;
            break;
        case SDK_OBJ_PORT:
            max_count = PORT_MAX_COUNT;
            break;
        default:
            return 1;
    }
    return (list_size == max_count);
}

uint32_t add_object(const object_type_t key, attr_t* attr_list, const uint32_t attr_count)
{
    if (current_objects_table == 0)
    {
        current_objects_table = create_table();
    }

    objects_table_entry_t* list = current_objects_table->head;
    while (list != 0)
    {
        if ((list->key == key))
        {
            if (is_max_objects(list, key) != 0)
            {
                printf("can't put more than %d of object %d\n", list->objects->size, key);
                return 0;
            }
            printf("list size = %d\n", list->objects->size);
            return add_node(list->objects, attr_list, attr_count);
        }

        list = list->next;
    }
    printf("entry with key %d isn't found in the objects table, create new entry\n", key);

    object_list_t* new_list = create_list();
    add_entry(key, new_list);

    const uint32_t object_id = add_node(new_list, attr_list, attr_count);

    return object_id;
}

object_list_entry_t* get_object(const object_type_t key, const uint32_t id)
{
    objects_table_entry_t* table_entry = get_entry(key);
    object_list_entry_t* object = get_node(table_entry->objects, id);

    return object;
}

operation_result_t delete_object(const object_type_t key, const uint32_t id)
{
    objects_table_entry_t* entry = get_entry(key);
    return delete_node(entry->objects, id);
}

operation_result_t delete_entry(const object_type_t key)
{
    objects_table_entry_t* entry;
	objects_table_entry_t* prev;
	
	for (entry = current_objects_table->head; (entry != 0 && entry->key != key); entry = entry->next)
	{
		prev = entry;
	}
	
	if (entry == 0)
	{
		printf("entry's not found");
		return RSLT_FAILURE;
	}
	
	if (prev)
	{
		if (entry == current_objects_table->head) 
		{
			current_objects_table->head = entry->next;
		} 
		else 
		{
			prev->next = entry->next;
		}
        delete_list(entry->objects);
		free(entry);

        return RSLT_SUCCESS;
	}
    else
    {
        printf("delete_node: no prev node\n");
    }
}

