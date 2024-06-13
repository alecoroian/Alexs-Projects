
#include <stdio.h>
#include <stdlib.h>



typedef struct node {
	void*  data;
	struct node* left;
	struct node* right;
} node_t;

typedef struct tree {
	node_t* root;
	void (*add_node)(void*, size_t, struct tree*, int (*)(void*,void*));
	void (*print_tree)(node_t*, void (*)(void*));
	void (*destroy)(struct tree*);
} tree_t;


void add_node(void* , size_t, tree_t*, int (*)(void*,void*));

void print_tree(node_t*, void (*)(void*));

void destroy(tree_t*);