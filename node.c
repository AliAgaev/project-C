#include "../project 4/node.h"

node *createNode(char letter, uint64_t frequency) {
    node *n = (node *) (malloc(sizeof(node)));
    n->letter = letter;
    n->frequency = frequency;
    n->left = NULL;
    n->right = NULL;
    return n;
}

void swap(node *a, node *b) {
    node temp = *b;
    *b = *a;
    *a = temp;
}
