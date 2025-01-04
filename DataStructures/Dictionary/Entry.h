#ifndef Entry_h
#define Entry_h

// Data Types

struct Entry
{
		void *key;
		void *value;
};

// CONSTRUCTORS
struct Entry entry_constructor(void *key, unsigned long key_size, void *value, unsigned long value_size);
void entry_destructor(struct Entry *entry);
#endif
