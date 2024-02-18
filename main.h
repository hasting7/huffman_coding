#ifndef MAIN_H
#define MAIN_H

#define MAX_MESSAGE 2028

struct huffman_struct *get_huffman_from_heap(void *);
int get_weight(void *);
int compare_elements(void *, void *);
struct heap_struct *create_huffman_queue(char *);
void huffman_translate(char **, struct tree_node *, char *, char *);
char *add_char(char *, char);
char *decode(struct tree_node *, char *, char *);
char *encode(char **, char *);

struct huffman_struct {
	int weight;
	char character;
};


#endif