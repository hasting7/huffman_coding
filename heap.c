#include <stdlib.h>

#include "heap.h"

void swap(struct heap_object *, struct heap_object *);
void swim_up(struct heap_struct *, int);
void sink(struct heap_struct *, int);

struct heap_struct *create_heap(int (*value)(void *), int (*cmp)(void *, void *)) {
	Heap *new = malloc(sizeof(struct heap_struct));
	new->list = calloc(MAX_HEAP_SIZE, sizeof(struct heap_object));
	new->get_value = value;
	new->compare = cmp;
	new->size = 0;
	return new;
} 

void free_heap(struct heap_struct **heap) {
	struct heap_object *temp = (*heap)->list;
	for (int i = 0; i < (*heap)->size; i++) {
		free(temp[i].data);
	}
	free(temp);
	free(*heap);
	*heap = NULL;
}

void swim_up(struct heap_struct *heap, int index) {
	if (index == 0) return;
	int parent_index = (index - 1) / 2;
	if (heap->compare(heap->list[index].data, heap->list[parent_index].data) > 0) {
		swap(&heap->list[index], &heap->list[parent_index]);
	}
	swim_up(heap, parent_index);
}

void sink(struct heap_struct *heap, int index) {
	if (index > heap->size) return;
	int target = index;
	int child_i = index * 2;
	for (int i = 1; i < 3; i++) {
		if (child_i + i > heap->size - 1) break;
		if (heap->compare(heap->list[target].data, heap->list[child_i + i].data) < 0) {
			target = child_i + i;
		}
	}
	if (target == index) return;
	swap(&heap->list[index], &heap->list[target]);
	sink(heap, target);
}

void swap(struct heap_object *obj1, struct heap_object *obj2) {
	struct heap_object temp = *obj1;
	*obj1 = *obj2;
	*obj2 = temp;
}

void *pop(struct heap_struct *heap) {
	void * top = heap->list[0].data;
	swap(&heap->list[0], &heap->list[--heap->size]);
	sink(heap, 0);
	return top;
}


void heap_add(struct heap_struct *heap, void *data) {
	object *container = malloc(sizeof(struct heap_object));
	container->data = data;
	container->key = heap->get_value(data);
	heap->list[heap->size] = *container;
	swim_up(heap, heap->size);
	heap->size++;
}
