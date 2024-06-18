#ifndef ALICOMPRESSOR_NODE_H
#define ALICOMPRESSOR_NODE_H

#include <stdint.h>
#include <stdlib.h>

typedef struct m_node {
    uint64_t frequency;
    char letter;
    struct m_node *left;
    struct m_node *right;
} node;

node *createNode(char letter, uint64_t frequency);

void swap(node *a, node *b);

#endif //ALICOMPRESSOR_NODE_H
