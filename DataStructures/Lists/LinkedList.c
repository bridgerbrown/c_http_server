#include <stdlib.h>
#include <stdio.h>

#include "LinkedList.h"

// FUNCTION PROTOTYPES
// Private Member Methods
struct Node * create_node_ll(void *data, unsigned long size);
void destroy_node_ll(struct Node *node_to_destroy);


// Public Member Methods
struct Node * iterate_ll(struct LinkedList *linked_list, int index);
void insert_ll(struct LinkedList *linked_list, int index, void *data, unsigned long size);
void remove_node_ll(struct LinkedList *linked_list, int index);
void * retrieve_ll(struct LinkedList *linked_list, int index);
void bubble_sort_ll(struct LinkedList *linked_list, int (*compare)(void *a, void *b));
short binary_search_ll(struct LinkedList *linked_list, void *query, int (*compare)(void *a, void *b));

// CONSTRUCTORS
struct LinkedList linked_list_constructor()
{
    struct LinkedList new_list;
    new_list.head = NULL;
    new_list.length = 0;
    
    new_list.insert = insert_ll;
    new_list.remove = remove_node_ll;
    new_list.retrieve = retrieve_ll;
    new_list.sort = bubble_sort_ll;
    new_list.search = binary_search_ll;
    
    return new_list;
}

void linked_list_destructor(struct LinkedList *linked_list)
{
    for (int i = 0; i < linked_list->length; i++)
    {
        linked_list->remove(linked_list, 0);
    }
}



// PRIVATE METHODS
struct Node * create_node_ll(void *data, unsigned long size)
{
    // Allocate space.
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    // Call the constructor.
    *new_node = node_constructor(data, size);
    return new_node;
}

void destroy_node_ll(struct Node *node_to_destroy)
{
    node_destructor(node_to_destroy);
}

struct Node * iterate_ll(struct LinkedList *linked_list, int index)
{
    if (index < 0 || index >= linked_list->length)
    {
        return NULL;
    }
    struct Node *current = linked_list->head;
    for (int i = 0; i < index; i++)
    {
        current = current->next;
    }
    return current;
}



// PUBLIC METHODS
void insert_ll(struct LinkedList *linked_list, int index, void *data, unsigned long size)
{
    struct Node *node_to_insert = create_node_ll(data, size);
    if (index == 0)
    {
        node_to_insert->next = linked_list->head;
        linked_list->head = node_to_insert;
    }
    else
    {
        struct Node *current = iterate_ll(linked_list, index - 1);
        node_to_insert->next = current->next;
        current->next = node_to_insert;
        
    }
    linked_list->length += 1;
}

void remove_node_ll(struct LinkedList *linked_list, int index)
{
    if (index == 0)
    {
        struct Node *node_to_remove = linked_list->head;
        if (node_to_remove)
        {
            linked_list->head = node_to_remove->next;
            destroy_node_ll(node_to_remove);
        }
    }
    else
    {
        struct Node *current= iterate_ll(linked_list, index - 1);
        struct Node *node_to_remove = current->next;
        current->next = node_to_remove->next;
        destroy_node_ll(node_to_remove);
    }
    linked_list->length -= 1;
}

void * retrieve_ll(struct LinkedList *linked_list, int index)
{
    struct Node *current = iterate_ll(linked_list, index);
    if (current)
    {
        return current->data;
    }
    else
    {
        return NULL;
    }
}

void bubble_sort_ll(struct LinkedList *linked_list, int (*compare)(void *a, void *b))
{
    for (struct Node *i = linked_list->retrieve(linked_list, 0); i; i = i->next)
    {
        for (struct Node *n = i->next; n; n = n->next)
        {
            if (compare(i->data, n->data) > 0)
            {
                void *temporary = n->data;
                n->data = i->data;
                i->data = temporary;
            }
        }
    }
}

short binary_search_ll(struct LinkedList *linked_list, void *query, int (*compare)(void *a, void *b))
{
    int position = linked_list->length/2;
    int min_checked = 0;
    int max_checked = linked_list->length;
    while (max_checked > min_checked)
    {
        void *data = linked_list->retrieve(linked_list, position);
        if (compare(data, query) == 1)
        {
            max_checked = position;
            if (position != (min_checked + position) / 2)
            {
                position = (min_checked + position) / 2;
            }
            else
            {
                break;
            }
        }
        else if (compare(data, query) == -1)
        {
            min_checked = position;
            if (position != (max_checked + position) / 2)
            {
                position = (max_checked + position) / 2;
            }
            else
            {
                break;
            }
        }
        else
        {
            return 1;
        }
    }
    return 0;
}
