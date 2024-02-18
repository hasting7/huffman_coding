#include <stdlib.h>

#include "tree.h"

struct tree_node *create_node(void * data) {
	struct tree_node *new = malloc(sizeof(struct tree_node));
	new->children = calloc(2, sizeof(void *));
	new->data = data;
	return new;
}



// FIX FREE

void free_tree(struct tree_node **root, void (*free_func)(void *)) {
	struct tree_node *node = *root;
	if (node == NULL) return;

	free_tree(&(node->children[0]), free_func);
	free_tree(&(node->children[1]), free_func);

	free_func(node->data);
	free(node->children);
	free(node);
	*root = NULL;
}
