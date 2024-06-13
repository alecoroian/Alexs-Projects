/*******************************************************************************
 * Name        : bstree.c
 * Author      : Alex Coroian
 * Pledge      : I pledge my honour that I have abided by the Stevens Honour System
 ******************************************************************************/
#include "bstree.h"

void add_node(void *data, size_t data_size, tree_t *tree, int (*cmp)(void *, void *))
{
    node_t *node = (node_t *)malloc(sizeof(node_t));
    node->data = malloc(data_size);
    node->left = node->right = NULL;

    char *data_dest = (char *)node->data;
    char *data_src = (char *)data;

    for (size_t i = 0; i < data_size; ++i)
    {
        data_dest[i] = data_src[i];
    }

    node_t **place = &(tree->root);

    while (*place != NULL)
    {
        place = cmp(data, (*place)->data) < 0 ? &((*place)->left) : &((*place)->right);
    }

    *place = node;
}

void destroy_tree(node_t *node)
{
    if (!node)
    {
        return;
    }
    if (node->left)
    {
        destroy_tree(node->left);
    }
    if (node->right)
    {
        destroy_tree(node->right);
    }
    free(node->data);
    free(node);
}

void destroy(tree_t *tree)
{
    if (tree)
    {
        destroy_tree(tree->root);
        tree->root = NULL;
    }
}

void destroy_wrapper(node_t **nodePointer)
{
    if (nodePointer == NULL || *nodePointer == NULL)
    {
        return;
    }

    tree_t temporaryTree;
    temporaryTree.root = *nodePointer;
    destroy(&temporaryTree);
    *nodePointer = NULL;
}

void print_tree(node_t *node, void (*print)(void *))
{
    if (node == NULL)
    {
        return;
    }

    print_tree(node->left, print);
    print(node->data);
    print_tree(node->right, print);
}
