#include "sdk_object_list.h"
#include <stdio.h>

static uint32_t object_id_generator = 0;
object_list_t* current_object_list;
object_list_t* get_object_list()
{
    return current_object_list;
}

object_list_entry_t* create_node(const object_type_t object_type, attr_t* attr_list)
{
	object_list_entry_t* node;
	node = (object_list_entry_t*) malloc(sizeof(object_list_entry_t));
	if (node)
	{
		node->next = 0;
		node->object_type = object_type;
        node->attr_list = attr_list;
        node->id = ++object_id_generator;
        printf("create_node: id is %d\n", node->id);

		return node;
	}
	printf("create_node: node is null");
	return 0;
}

object_list_t* create_list()
{
	current_object_list = (object_list_t*) malloc(sizeof(object_list_t));
	if (current_object_list)
	{
		current_object_list->head = 0;
		return current_object_list;
	}
	return 0;
}


uint32_t add_node(object_list_t* list, const object_type_t object_type, attr_t* attr_list)
{
	if (list)
	{
		object_list_entry_t* node;
		node = create_node(object_type, attr_list);
		if (node)
		{
			node->next = list->head;
			list->head = node;
			return node->id;
		}
		printf("add_node: node is null\n");
		return 0;
	}
	printf("add_node: list is null\n");
    return 0;
}


void delete_node(object_list_t* list, const int id)
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
		return;
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
	}
}

object_list_entry_t* get_node(object_list_t* list, const int id)
{
    object_list_entry_t* node = list->head;
    while (node != 0)
    {
        if (node->id = id)
        {
            return node;
        }
        node = node->next;
    }
    return 0;
}

void print_list(object_list_t* list)
{
	object_list_entry_t* head = list->head;
	while(head != 0)
	{
		printf("%d ", head->id);
		head = head->next;
	}
	printf("\n");
}