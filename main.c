#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "heap.h"
#include "tree.h"
#include "main.h"

#define DISPLAY


struct huffman_struct *get_huffman_from_heap(void *data) {
	struct tree_node *converted = (struct tree_node *) data;
	return ((struct huffman_struct *) converted->data);
}

int get_weight(void *data) {
	struct tree_node *converted = (struct tree_node *) data;
	return ((struct huffman_struct *) converted->data)->weight;
}

int compare_elements(void *d1, void *d2) {
	return get_weight(d2) - get_weight(d1);
}

Heap *create_huffman_queue(char *string) {
	struct heap_struct *heap = create_heap(&get_weight, &compare_elements);
	int count[128] = { 0 };
	for (char *c = string; *c != '\0'; c++) {
		count[(int) *c]++;
	}
	struct huffman_struct *add;
	struct tree_node *node;
	for (int i = 0; i < 128; i++) {
		if (count[i] == 0) continue;
		add = malloc(sizeof(struct huffman_struct));
		add->weight = count[i];
		add->character = (char) i;
		node = create_node((void *) add);
		heap_add(heap, (void *) node);
	}
	return heap;
}

Node *huffman_construction(struct heap_struct *heap) {
	struct tree_node *p1;
	struct tree_node *p2;
	struct tree_node *new;
	struct huffman_struct *new_content;

	while (heap->size > 1) {
		p1 = (struct tree_node *) pop(heap);
		p2 = (struct tree_node *) pop(heap);
		new_content = malloc(sizeof(struct huffman_struct));
		new_content->weight = ((struct huffman_struct *) p1->data )->weight + ((struct huffman_struct *) p2->data )->weight;
		new_content->character = '\0';
		new = create_node((void *) new_content);
		new->children[1] = p1;
		new->children[0] = p2;
		heap_add(heap, (void *) new);
	}

	return (struct tree_node *) heap->list[0].data;

}

char *add_char(char *src, char add) {
	int len = strlen(src) + 2;
	char *new = malloc(len * sizeof(char));
	strcpy(new, src);
	new[len - 2] = add;
	return new;
}

void huffman_translate(char **out, struct tree_node *root, char *str, char *map) {
	char *left = add_char(str, map[0]);
	char *right = add_char(str, map[1]);

	if (root == NULL) return;
	struct huffman_struct *content = (struct huffman_struct *) root->data;

	if (content->character == '\0') {
		huffman_translate(out, root->children[0], left, map);
		huffman_translate(out, root->children[1], right, map);
	} else {
		char *code = malloc(strlen(str) + 1 * sizeof(char));
		strncpy(code, str, strlen(str) + 1);
		out[(int)content->character] = code;
	}

	free(left);
	free(right);
}

char * encode(char **map, char *message) {
	char *resp = malloc(sizeof(char) * MAX_MESSAGE);
	for (char *c = message; *c != '\0'; c++) {
		if (map[(int) *c] == NULL) {
			free(resp);
			printf("%d not in map\n", *c);
			return NULL;
		}
		strcat(resp, map[(int) *c]);
	}
	return resp;
}

char * decode(struct tree_node *root, char *message, char *map) {
	struct tree_node *walk = root;
	struct huffman_struct *data;
	char *resp = malloc(sizeof(char) * MAX_MESSAGE);
	for (char *c = message; *c != '\0'; c++) {
		if (strncmp(c, map, 1) == 0) { // left
			walk = walk->children[0];
		} else { // right
			walk = walk->children[1];
		}
		data = (struct huffman_struct *) walk->data;
		if (data->character != '\0') {
			resp[strlen(resp) + 1] = '\0';
			resp[strlen(resp)] = data->character;
			walk = root;
		}
	}
	return resp;
}

char *read_file(FILE *fp) {
	fseek(fp, 0, SEEK_END);
	long len = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char * content = malloc(len + 1 * sizeof(char));
	fread(content,len,1,fp);
	return content;
}


// input_file map {encode/decode} message 
int main(int argc, char **argv) {
	FILE *input;
	FILE *message;
	char *sample;
	char *string;
	char *map;
	if (argc != 5) {
		printf("Usage: ./huffman input_file {map} {e/d} message_file\n");
		return 1;
	}
	map = argv[2];
	if (strlen(map) != 2) {
		printf("map should only be two characters.\n");
		return 1;
	}
	input = fopen(argv[1],"r");
	if (!input) {
		printf("input file does not exist.\n");
		return 1;
	}
	message = fopen(argv[4], "r");
	if (!message) {
		printf("output file does not exist.\n");
		fclose(input);
		return 1;
	}

	

	sample = read_file(input);
	string = read_file(message);

	// HUFFMAN CODING
	Heap *heap = create_huffman_queue(sample);
	Node *tree = huffman_construction(heap);
	char **output = calloc(128, sizeof(char *));
	huffman_translate(output, tree, "", map);

#if define DISPLAY
	for (int i = 0; i < 128; i++) {
		if (output[i] == NULL) continue;
		printf("%c : %s\n", i, output[i]);
	}
#endif

	char *resp;
	if (strncmp(argv[3],"e",1) == 0) {
		resp = encode(output, string);
	} else {
		resp = decode(tree, string, map);
	}

	printf("%s\n", resp);

	// FREE
	free_heap(&heap);
	for (int i = 0; i < 128; i++) {
		if (output[i] == NULL) continue;
		free(output[i]);
	}
	free(sample);
	free(string);
	free(output);
	fclose(input);
	fclose(message);

	// free_tree(&tree, &free);

	return 0;
}