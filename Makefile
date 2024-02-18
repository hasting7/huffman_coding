goal: final

final:
	gcc -Wall -std=c17 -o huffman heap.c tree.c main.c
