#ifndef TREE_H
#define TREE_H

typedef struct tree_node {
	struct tree_node **children;
	void *data;
} Node;

struct tree_node *create_node(void *);
void free_tree(struct tree_node **, void (*)(void *));

#endif