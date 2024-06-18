#ifndef ALICOMPRESSOR_HUFFMAN_COMPRESSING_H
#define ALICOMPRESSOR_HUFFMAN_COMPRESSING_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "node.h"
#include "priority_queue.h"

void encode(node *root, const char *str, char **huffmanCode);

void decode(node *root, int *index, const char *str, FILE* f);

void buildHuffmanTree(const char *string);

void parseFile(const char* filepath);

int getBit(unsigned char byte, int pos);

int binaryToDecimal(unsigned char byte);

void addNodeDecompress(node* root, const char* code, char letter, int current_index);

node* createTreeFromCodes( char** huffmanCodes);

void decompressHuffman(const char* filepath);

#endif //ALICOMPRESSOR_HUFFMAN_COMPRESSING_H
