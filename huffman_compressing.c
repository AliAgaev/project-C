#include "../project 4/huffman_compressing.h"

void encode(node *root, const char *str,
            char **huffmanCode) {
    if (root == NULL) {
        return;
    }

    if (!root->left && !root->right) {
        huffmanCode[(int) (root->letter)] = (char *) (malloc(sizeof(char) * strlen(str) + 1));
        strcpy(huffmanCode[(int) (root->letter)], str);
    }

    char *left = (char *) (malloc(sizeof(char) * strlen(str) + 2));
    strcpy(left, str);
    strcat(left, "0");
    encode(root->left, left, huffmanCode);
    free(left);

    char *right = (char *) (malloc(sizeof(char) * strlen(str) + 2));;
    strcpy(right, str);
    strcat(right, "1");
    encode(root->right, right, huffmanCode);
    free(right);
}

void decode(node *root, int *index, const char *str, FILE* f) {
    if (root == NULL) {
        return;
    }

    if (!root->left && !root->right) {
        printf("%c", root->letter);
        fprintf(f, "%c", root->letter);
        return;
    }

    (*index)++;

    if (str[*index] == '0') {
        decode(root->left, index, str, f);
    } else {
        decode(root->right, index, str, f);
    }
}

unsigned char codeToByte(const char *code) {
    unsigned char res = 0;
    int len = strlen(code);
    for (int i = 0; i < len; ++i) {
        if (code[i] == '1') {
            res |= (1 << i);
        }
    }
    return res;
}

void buildHuffmanTree(const char *string) {
    // Count frequency of characters: index - code of symbol, value - frequency of this symbol
    const int alphabet_size = (1 << (sizeof(char) * 8)); // 1 << sizeof(char) * 8 <=> 1 << 8 <=> pow(2, 8)
    uint64_t *frequency = (uint64_t *) (calloc(alphabet_size, sizeof(uint64_t)));
    for (uint64_t i = 0; i < strlen(string); ++i) {
        ++frequency[(int) (string[i])];
    }

    // Priority queue
    priority_queue *pq = createQueue();
    int unique_count = 0;
    for (int i = 0; i < alphabet_size; ++i) {
        if (frequency[i] > 0) {
            // priorityQueue Push
            insert(pq, createNode((char) (i), frequency[i]));
            ++unique_count;
        }
    }

    while (pq->size != 1) {
        node *left = createNode(pq->data[0]->letter, pq->data[0]->frequency);
        left->left = pq->data[0]->left;
        left->right = pq->data[0]->right;
        deleteRoot(pq, pq->data[0]->letter);

        node *right = createNode(pq->data[0]->letter, pq->data[0]->frequency);
        right->left = pq->data[0]->left;
        right->right = pq->data[0]->right;
        deleteRoot(pq, pq->data[0]->letter);

        uint64_t sum = left->frequency + right->frequency;

        node *new_node = createNode('\0', sum);
        new_node->left = left;
        new_node->right = right;

        insert(pq, new_node);
    }

    node *root = pq->data[0];

    char **huffmanCode = (char **) (calloc(alphabet_size + 1, sizeof(char *)));
    encode(root, "", huffmanCode);

//    printf("Huffman Codes are:\n");
    FILE *encodedFile = fopen("encoded.zip", "wb");
    if (encodedFile == NULL) {
        printf("encodedFile can not be opened.");
        return;
    }
    fwrite(&unique_count, 1, 1, encodedFile);
    for (int i = 0; i < alphabet_size; ++i) {
        if (huffmanCode[i] != NULL) {
            printf("%c: %s\n", (char) (i), huffmanCode[i]);
            unsigned char len = strlen(huffmanCode[i]);
            unsigned char symbol = (char) (i);
            unsigned char code = codeToByte(huffmanCode[i]);
            fwrite(&len, 1, 1, encodedFile);
            fwrite(&symbol, 1, 1, encodedFile);
            fwrite(&code, 1, 1, encodedFile);
        }
    }

//    printf("\nOriginal string was :\n%s\n", string);

//    printf("\nEncoded string is :\n");
    char *encoded = (char *) (calloc(1000000, sizeof(char)));
    for (int i = 0; i < strlen(string); ++i) {
        strcat(encoded, huffmanCode[(int) (string[i])]);
//        printf("%s", huffmanCode[(int)(string[i])]);
//        fprintf(encodedFile, "%s", huffmanCode[(int)(string[i])]);
    }
    int offset = 8 - strlen(encoded) % 8;
    fwrite(&offset, 1, 1, encodedFile);

    // write to binary
    for (int i = 0; i < strlen(encoded); i += 8) {
        unsigned char byte = 0;
        for (int j = 0; j < 8 && i + j < strlen(encoded); ++j) {
            if (encoded[i + j] == '1') {
                byte |= (1 << j);
            }
        }
        fwrite(&byte, 1, 1, encodedFile);
    }
    fclose(encodedFile);


    free(encoded);
    free(frequency);
    deallocateQueue(pq);
}

int getBit(unsigned char byte, int pos) {
    return (byte & (1 << pos)) ? 1 : 0;
}

int binaryToDecimal(unsigned char byte) {
    int res = 0;
    for (int i = 0; i < 8; ++i) {
        res += (getBit(byte, i)) * (1 << i);
    }
    return res;
}

void addNodeDecompress(node *root, const char *code, char letter, int current_index) {
    if (current_index + 1 == strlen(code)) {
        if (code[current_index] == '1') {
            root->right = createNode(letter, 1);
        } else {
            root->left = createNode(letter, 1);
        }
    } else {
        if (code[current_index] == '1') {
            if (root->right == NULL) {
                root->right = createNode('\0', 0);
            }
            addNodeDecompress(root->right, code, letter, current_index + 1);
        } else {
            if (root->left == NULL) {
                root->left = createNode('\0', 0);
            }
            addNodeDecompress(root->left, code, letter, current_index + 1);
        }
    }
}

node *createTreeFromCodes(char **huffmanCodes) {
    node *root = createNode('\0', 0);
    for (int i = 0; i < 256; ++i) {
        if (huffmanCodes[i] != NULL) {
            addNodeDecompress(root, huffmanCodes[i], (char) i, 0);
        }
    }
    return root;
}

void parseFile(const char *filepath) {
    FILE *f = fopen(filepath, "rb");
    const int alphabet_size = 256;
    char **huffmanCode = (char **) (calloc(alphabet_size + 1, sizeof(char *)));
    unsigned char byte;
    // read first byte
    fread(&byte, 1, 1, f);
    int unique_symbols_count = binaryToDecimal(byte);
    printf("Unique symbols: %d\n", unique_symbols_count);
    for (int i = 0; i < unique_symbols_count; ++i) {
        fread(&byte, 1, 1, f);
        int len = binaryToDecimal(byte);
        printf("len: %d\t", len);
//        for (int i = 7; i >=0; --i) {
//            printf("%d", getBit(byte, i));
//        }
        printf("\n");

        fread(&byte, 1, 1, f);
        unsigned char symbol = byte;
        printf("symbol: %c\n", byte);
//        for (int i = 7; i >=0; --i) {
//            printf("%d", getBit(byte, i));
//        }

        fread(&byte, 1, 1, f);
        printf("code: ");
        char *code = (char *) (calloc(9, sizeof(char)));
        for (int i = len - 1, j = 0; i >= 0; --i, ++j) {
            code[i] = getBit(byte, i) == 1 ? '1' : '0';
        }
        printf("%s", code);
        huffmanCode[(int) (symbol)] = code;
        printf("\n\n");
    }

    node *root = createTreeFromCodes(huffmanCode);

    // Read rest of file
    unsigned char offset_byte;
    fread(&offset_byte, 1, 1, f);
    printf("offset = %d\n", binaryToDecimal(offset_byte));

    char *encoded = (char *) (calloc(1000000, 1));
    unsigned char sym;
    int i = 0;
    while (!feof(f)) {
        fread(&sym, 1, 1, f);
        for (int j = 0; j < 8; ++j) {
            encoded[i++] = getBit(sym, j) == 0 ? '0' : '1';
        }
    }
    encoded[strlen(encoded) - binaryToDecimal(offset_byte) - 8] = '\0';
    int index = -1;
    printf("\nDecoded string is :\n");
//    for (int i = 0; i < strlen(encoded) / 2; ++i) {
//        char tmp = encoded[i];
//        encoded[i] = encoded[strlen(encoded) - 1 - i];
//        encoded[strlen(encoded) - 1 - i] = tmp;
//    }
FILE* res = fopen("decoded.txt", "w");
    while (index < (int) (strlen(encoded)) - 1) {
        decode(root, &index, encoded, res);
    }
    fclose(res);
    free(encoded);

    for (int i = 0; i < alphabet_size; ++i) {
        if (huffmanCode[i] != NULL) {
            free(huffmanCode[i]);
        }
    }

    free(huffmanCode);
    fclose(f);
}

void decompressHuffman(const char *filepath) {
    parseFile(filepath);
}
