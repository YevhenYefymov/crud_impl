#include "sdk_object_list.h"
#include <stdio.h>

static uint32_t object_id_generator = 0;

object_list_entry_t* create_node(attr_t* attr_list, const uint32_t attr_count)
{
	object_list_entry_t* node;
	node = (object_list_entry_t*) malloc(sizeof(object_list_entry_t));
	if (node)
	{
		node->next = 0;
        node->attr_list = attr_list;
        node->attr_count = attr_count;
        node->id = ++object_id_generator;

		return node;
	}
	printf("create_node: node is null");
	return 0;
}

object_list_t* create_list()
{
	object_list_t* list = (object_list_t*) malloc(sizeof(object_list_t));
	if (list)
	{
		list->head = 0;
		list->size = 0;
		return list;
	}
	return 0;
}

operation_result_t delete_list(object_list_t* list)
{
    object_list_entry_t* node = list->head;
    object_list_entry_t* next = 0;
    while (node != 0)
    {
        next = node->next;
        free(node);
        node = next;
    }

    free(list);
}

uint32_t add_node(object_list_t* list, attr_t* attr_list, const uint32_t attr_count)
{
	if (list)
	{
		object_list_entry_t* node;
		node = create_node(attr_list, attr_count);
		if (node)
		{
			node->next = list->head;
			list->head = node;
			list->size++;
			printf("add_node: new size = %d\n", list->size);
			return node->id;
		}
		printf("add_node: node is null\n");
		return 0;
	}
	printf("add_node: list is null\n");
    return 0;
}


operation_result_t delete_node(object_list_t* list, const int id)
{
	object_list_entry_t* node;
	object_list_entry_t* prev;
	
	for (node = list->head; (node != 0 && node->id != id); node = node->next)
	{
		prev = node;
	}
	
	if (node == 0)
	{
		printf("entry's not found");
		return RSLT_FAILURE;
	}
	
	if (prev)
	{
		if (node == list->head) 
		{
			list->head = node->next;
		} 
		else 
		{
			prev->next = node->next;
		}
		free(node);
		list->size--;

        return RSLT_SUCCESS;
	}
    else
    {
        printf("delete_node: no prev node\n");
    }
}

object_list_entry_t* get_node(object_list_t* list, const int id)
{
    object_list_entry_t* node = list->head;
    while (node != 0)
    {
        if (id == node->id)
        {
            return node;
        }
        node = node->next;
    }
    return 0;
}