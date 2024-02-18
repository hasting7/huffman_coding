#ifndef HEAP_H
#define HEAP_H

#define MAX_HEAP_SIZE 100

typedef struct heap_struct {
	struct heap_object *list;
	int (*compare)(void *, void *);
	int (*get_value)(void *);
	int size;
} Heap;

typedef struct heap_object {
	void *data;
	int key;
} object;


void heap_add(struct heap_struct *, void *);
struct heap_struct *create_heap(int (*value)(void *), int (*cmp)(void *, void *));
void free_heap(struct heap_struct **);
void *pop(struct heap_struct *);

#endif