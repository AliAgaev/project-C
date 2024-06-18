#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "headers/huffman_compressing.h"

#define BYTE_LEN 8

void showUsageMessage() {
    printf("Program name: Huffman compressor\n");
    printf("Version: 1.0\n");
    printf("Usage\n");
    printf("--------------------------------\n");
    printf("1. If you want to compress some text file, please run program with flag -c (compress) "
           "and provide some filename.\n\tExample: \"./prog -c file.txt\"\n");
    printf("2. If you want to decompress some text file, please run program with flag -d (decompress) "
           "and provide some filename.\n\tExample: \"./prog -d file.huf\"\n");
}

int main(int argc, char** argv) {
    if (argc < 3) {
        showUsageMessage();
        return 1;
    }
    if (strcmp(argv[1], "-c") == 0) {
        FILE* f = fopen(argv[2], "r");
        if (f == NULL) {
            printf("No such filename: %s", argv[2]);
            return 1;
        }
        fseek(f, 0L, SEEK_END);
        uint64_t pos = ftell(f);
        rewind(f);

        char* str = (char*)(malloc(sizeof(char) * pos + 1));
        fread(str, pos, sizeof (char), f);
        buildHuffmanTree(str);
        free(str);
        fclose(f);
    } else if (strcmp(argv[1], "-d") == 0) {
        decompressHuffman(argv[2]);
    } else {
        showUsageMessage();
    }
    return 0;
}
