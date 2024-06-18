#include "../project 4/binary_tree.h"

tree *createTree() {
    tree *t = (tree *) (malloc(sizeof(tree)));
    t->root = NULL;
    t->nodes_count = 0;
    return t;
}

void m_addNode(node *current_node, node *new_node) {
    if (current_node->left == NULL) {
        current_node = new_node;
    } else {
        if (current_node->left->frequency <= new_node->frequency) {
            current_node->right = new_node;
        } else {
            current_node->right = current_node->left;
            current_node->left = new_node;
        }
    }
    current_node->frequency += new_node->frequency;
}

void addNode(tree *t, node *new_node) {
    m_addNode(t->root, new_node);
}