#ifndef Queue_h
#define Queue_h

#include "LinkedList.h"

// DATA TYPES
struct Queue
{
		struct LinkedList list;
		
		void (*push)(struct Queue *queue, void *data, unsigned long size);
		void * (*peek)(struct Queue *queue);
		void (*pop)(struct Queue *queue);
};

// CONSTRUCTORS
struct Queue queue_constructor(void);
void queue_destructor(struct Queue *queue);
#endif
