#include "bstree.h"
#include "utils.h"

int main()
{

	tree_t thing;
	thing.add_node = &add_node;
	thing.root = NULL;

	int a = 10;
	thing.add_node(&a, sizeof(int), &thing, &cmpr_int);
	a = 20;
	thing.add_node(&a, sizeof(int), &thing, &cmpr_int);
	a = 5;
	thing.add_node(&a, sizeof(int), &thing, &cmpr_int);
	a = 1;
	thing.add_node(&a, sizeof(int), &thing, &cmpr_int);
	a = 3;
	thing.add_node(&a, sizeof(int), &thing, &cmpr_int);
	a = 15;
	thing.add_node(&a, sizeof(int), &thing, &cmpr_int);

	thing.print_tree = &print_tree;
	thing.print_tree(thing.root, &print_int);

	thing.destroy = &destroy;
	thing.destroy(&thing.root);
	thing.root = NULL;

	thing.print_tree(thing.root, &print_int);
}
