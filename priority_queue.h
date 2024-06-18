#ifndef ALICOMPRESSOR_PRIORITY_QUEUE_H
#define ALICOMPRESSOR_PRIORITY_QUEUE_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "node.h"

enum ReallocateType {
    INCREASE,
    DECREASE,
};

typedef struct pq {
    node **data;
    uint64_t capacity;
    uint64_t size;
} priority_queue;

priority_queue *createQueue();

void heapify(priority_queue *q, int64_t i);

void insert(priority_queue *q, node *newNum);

void deleteRoot(priority_queue *q, char letter);

void deallocateQueue(priority_queue *q);

void printArray(priority_queue *q);

void reallocateMemory(priority_queue *q, enum ReallocateType type);

#endif //ALICOMPRESSOR_PRIORITY_QUEUE_H
