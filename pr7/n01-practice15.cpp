#pragma warning(disable:4996)

#include "stdafx.h"

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <ctype.h>

#include <stdarg.h>

#define END_OF_STREAM 256

#define ESCAPE 257

#define SYMBOL_COUNT 258

#define NODE_TABLE_COUNT ((SYMBOL_COUNT * 2) - 1)

#define ROOT_NODE 0

#define MAX_WEIGHT 0x8000

#define TRUE 1

#define FALSE 0

#define PACIFIER_COUNT 2047

#define NO_ERROR 0

#define BAD_FILE_NAME 1

#define BAD_ARGUMENT 2

typedef struct bit_file {

	FILE* file;

	unsigned char mask;

	int rack;

	int pacifier_counter;

}

COMPRESSED_FILE;

struct node {

	unsigned int weight;

	int parent;

	int child_is_leaf;

	int child;

};

typedef struct tree {

	int leaf[SYMBOL_COUNT];

	int next_free_node;

	node nodes[NODE_TABLE_COUNT];

}

TREE;

COMPRESSED_FILE* OpenOutputCompressedFile(char* name);

void OutputBit(COMPRESSED_FILE*, int bit);

void OutputBits(COMPRESSED_FILE* bit_file, unsigned long code, int count);

void CloseOutputCompressedFile(COMPRESSED_FILE* bit_file);

void CompressFile(FILE* input, COMPRESSED_FILE* output);

void InitializeTree(TREE* tree);

void EncodeSymbol(TREE* tree, unsigned int c, COMPRESSED_FILE* output);

void UpdateModel(TREE* tree, int c);

void RebuildTree(TREE* tree);

void swap_nodes(TREE* tree, int i, int j);

void add_new_node(TREE* tree, int c);

#ifndef SEEK_END

#define SEEK_END 2

#endif

COMPRESSED_FILE* OpenOutputCompressedFile(char* name) {

	COMPRESSED_FILE* compressed_file;

	compressed_file = (COMPRESSED_FILE*)

		calloc(1, sizeof(COMPRESSED_FILE));

	if (compressed_file == NULL)

		return(compressed_file);

	compressed_file->file = fopen(name, "wb");

	compressed_file->rack = 0;

	compressed_file->mask = 0x80;

	compressed_file->pacifier_counter = 0;

	return(compressed_file);

}

void CloseOutputCompressedFile(COMPRESSED_FILE* compressed_file) {

	fclose(compressed_file->file);

	free((char*)compressed_file);

}

void OutputBit(COMPRESSED_FILE* compressed_file, int bit) {

	if (bit)

		compressed_file->rack |= compressed_file->mask;

	compressed_file->mask >>= 1;

	if (compressed_file->mask == 0) {

		if (putc(compressed_file->rack, compressed_file->file) != compressed_file->rack)

			printf("Error on OutputBit!\n");

		else if ((compressed_file->pacifier_counter++ & PACIFIER_COUNT) == 0)

			putc('.', stdout);

		compressed_file->rack = 0;

		compressed_file->mask = 0x80;

	}

}

void OutputBits(COMPRESSED_FILE* compressed_file, unsigned long code, int count) {

	unsigned long mask;

	mask = 1L << (count - 1);

	while (mask != 0) {

		if (mask & code)

			compressed_file->rack |= compressed_file->mask;

		compressed_file->mask >>= 1;

		if (compressed_file->mask == 0) {

			if (putc(compressed_file->rack, compressed_file->file) != compressed_file->rack)

				printf("Error on OutputBits!\n");

			else if ((compressed_file->pacifier_counter++ & PACIFIER_COUNT) == 0)

				putc('.', stdout);

			compressed_file->rack = 0;

			compressed_file->mask = 0x80;

		}

		mask >>= 1;

	}

}

TREE Tree;

void CompressFile(FILE* input, COMPRESSED_FILE* output) {

	int c;

	InitializeTree(&Tree);

	while ((c = getc(input)) != EOF) {

		EncodeSymbol(&Tree, c, output);

		UpdateModel(&Tree, c);

	}

	EncodeSymbol(&Tree, END_OF_STREAM, output);

}

void InitializeTree(TREE* tree) {

	int i;

	tree->nodes[ROOT_NODE].child = ROOT_NODE + 1;

	tree->nodes[ROOT_NODE].child_is_leaf = FALSE;

	tree->nodes[ROOT_NODE].weight = 2;

	tree->nodes[ROOT_NODE].parent = -1;

	tree->nodes[ROOT_NODE + 1].child = END_OF_STREAM;

	tree->nodes[ROOT_NODE + 1].child_is_leaf = TRUE;

	tree->nodes[ROOT_NODE + 1].weight = 1;

	tree->nodes[ROOT_NODE + 1].parent = ROOT_NODE;

	tree->leaf[END_OF_STREAM] = ROOT_NODE + 1;

	tree->nodes[ROOT_NODE + 2].child = ESCAPE;

	tree->nodes[ROOT_NODE + 2].child_is_leaf = TRUE;

	tree->nodes[ROOT_NODE + 2].weight = 1;

	tree->nodes[ROOT_NODE + 2].parent = ROOT_NODE;

	tree->leaf[ESCAPE] = ROOT_NODE + 2;

	tree->next_free_node = ROOT_NODE + 3;

	for (i = 0; i < END_OF_STREAM; i++)

		tree->leaf[i] = -1;

}

void EncodeSymbol(TREE* tree, unsigned int c,

	COMPRESSED_FILE* output) {

	unsigned long code;

	unsigned long current_bit;

	int code_size;

	int current_node;

	code = 0;

	current_bit = 1;

	code_size = 0;

	current_node = tree->leaf[c];

	if (current_node == -1)

		current_node = tree->leaf[ESCAPE];

	while (current_node != ROOT_NODE) {

		if ((current_node & 1) == 0)

			code |= current_bit;

		current_bit <<= 1;

		code_size++;

		current_node = tree->nodes[current_node].parent;

	}

	OutputBits(output, code, code_size);

	if (tree->leaf[c] == -1) {

		OutputBits(output, (unsigned long)c, 8);

		add_new_node(tree, c);

	}

}

void UpdateModel(TREE* tree, int c) {

	int current_node;

	int new_node;

	if (tree->nodes[ROOT_NODE].weight == MAX_WEIGHT)

		RebuildTree(tree);

	current_node = tree->leaf[c];

	while (current_node != -1) {

		tree->nodes[current_node].weight++;

		for (new_node = current_node; new_node > ROOT_NODE; new_node--)

			if (tree->nodes[new_node - 1].weight >= tree->nodes[current_node].weight)

				break;

		if (current_node != new_node) {

			swap_nodes(tree, current_node, new_node);

			current_node = new_node;

		}

		current_node = tree->nodes[current_node].parent;

	}

}

void RebuildTree(TREE* tree) {

	int i;

	int j;

	int k;

	unsigned int weight;

	printf("Loading");

	j = tree->next_free_node - 1;

	for (i = j; i >= ROOT_NODE; i--) {

		if (tree->nodes[i].child_is_leaf) {

			tree->nodes[j] = tree->nodes[i];

			tree->nodes[j].weight = (tree->nodes[j].weight + 1) / 2;

			j--;

		}

	}

	for (i = tree->next_free_node - 2; j >= ROOT_NODE; i -= 2, j--) {

		k = i + 1;

		tree->nodes[j].weight = tree->nodes[i].weight + tree->nodes[k].weight;

		weight = tree->nodes[j].weight;

		tree->nodes[j].child_is_leaf = FALSE;

		for (k = j + 1; weight < tree->nodes[k].weight; k++);

		k--;

		memmove(&tree->nodes[j], &tree->nodes[j + 1],

			(k - j) * sizeof(struct node));

		tree->nodes[k].weight = weight;

		tree->nodes[k].child = i;

		tree->nodes[k].child_is_leaf = FALSE;

	}

	for (i = tree->next_free_node - 1; i >= ROOT_NODE; i--) {

		if (tree->nodes[i].child_is_leaf) {

			k = tree->nodes[i].child;

			tree->leaf[k] = i;

		}

		else {

			k = tree->nodes[i].child;

			tree->nodes[k].parent = tree->nodes[k + 1].parent = i;

		}

	}

}

void swap_nodes(TREE* tree, int i, int j) {

	node temp;

	if (tree->nodes[i].child_is_leaf)

		tree->leaf[tree->nodes[i].child] = j;

	else {

		tree->nodes[tree->nodes[i].child].parent = j;

		tree->nodes[tree->nodes[i].child + 1].parent = j;

	}

	if (tree->nodes[j].child_is_leaf)

		tree->leaf[tree->nodes[j].child] = i;

	else {

		tree->nodes[tree->nodes[j].child].parent = i;

		tree->nodes[tree->nodes[j].child + 1].parent = i;

	}

	temp = tree->nodes[i];

	tree->nodes[i] = tree->nodes[j];

	tree->nodes[i].parent = temp.parent;

	temp.parent = tree->nodes[j].parent;

	tree->nodes[j] = temp;

}

void add_new_node(TREE* tree, int c) {

	int lightest_node;

	int new_node;

	int zero_weight_node;

	lightest_node = tree->next_free_node - 1;

	new_node = tree->next_free_node;

	zero_weight_node = tree->next_free_node + 1;

	tree->next_free_node += 2;

	tree->nodes[new_node] = tree->nodes[lightest_node];

	tree->nodes[new_node].parent = lightest_node;

	tree->leaf[tree->nodes[new_node].child] = new_node;

	tree->nodes[lightest_node].child = new_node;

	tree->nodes[lightest_node].child_is_leaf = FALSE;

	tree->nodes[zero_weight_node].child = c;

	tree->nodes[zero_weight_node].child_is_leaf = TRUE;

	tree->nodes[zero_weight_node].weight = 0;

	tree->nodes[zero_weight_node].parent = lightest_node;

	tree->leaf[c] = zero_weight_node;

}

int main() {

	COMPRESSED_FILE* output;

	FILE* input;

	input = fopen("D:\\aod14\\input.txt", "rb");

	output = OpenOutputCompressedFile((char*)"D:\\aod14\\output");

	CompressFile(input, output);

	CloseOutputCompressedFile(output);

	fclose(input);

	std::cout << "Completed.\n";

	return (NO_ERROR);

}