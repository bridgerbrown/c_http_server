#ifndef LinkedList_h
#define LinkedList_h

#include "../Common/Node.h"

// DATA TYPES
struct LinkedList
{
    /* PUBLIC MEMBER VARIABLES */
    struct Node *head;
    int length;
    
    /* PUBLIC MEMBER METHODS */
    void (*insert)(struct LinkedList *linked_list, int index, void *data, unsigned long size);
    void (*remove)(struct LinkedList *linked_list, int index);
    void * (*retrieve)(struct LinkedList *linked_list, int index);
    void (*sort)(struct LinkedList *linked_list, int (*compare)(void *a, void *b));
    short (*search)(struct LinkedList *linked_list, void *query, int (*compare)(void *a, void *b));
};

// CONSTRUCTORS 
struct LinkedList linked_list_constructor(void);
void linked_list_destructor(struct LinkedList *linked_list);

#endif 
