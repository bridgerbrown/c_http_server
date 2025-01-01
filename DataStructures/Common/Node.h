#ifndef Node_h
#define Node_h

// DATA TYPES
struct Node
{
    void *data;
    struct Node *next;
    struct Node *previous;
};

// CONSTRUCTORS
struct Node node_constructor(void *data, unsigned long size);
void node_destructor(struct Node *node);

#endif 
