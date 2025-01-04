#ifndef Dictionary_h
#define Dictionary_h

#include "../Trees/BinarySearchTree.h"
#include "../Lists/LinkedList.h"
#include "Entry.h"

// DATA TYPES
struct Dictionary
{
		struct BinarySearchTree binary_search_tree;
		struct LinkedList keys;
		void (*insert)(struct Dictionary *dictionary, void *key, unsigned long key_size, void *value, unsigned long value_size);
		void * (*search)(struct Dictionary *dictionary, void *key, unsigned long key_size);
};

// CONSTRUCTORS
struct Dictionary dictionary_constructor(int (*compare)(void *entry_one, void *entry_two));
void dictionary_destructor(struct Dictionary *dictionary);

// PUBLIC HELPER FUNCTIONS
int compare_string_keys(void *entry_one, void *entry_two);
#endif
