#ifndef ALICOMPRESSOR_BINARY_TREE_H
#define ALICOMPRESSOR_BINARY_TREE_H

#include <stdint.h>
#include <stdlib.h>
#include "node.h"

typedef struct m_tree {
    node *root;
    uint64_t nodes_count;
} tree;

tree *createTree();

void m_addNode(node *current_node, node *new_node);

void addNode(tree *t, node *new_node);

#endif //ALICOMPRESSOR_BINARY_TREE_H
