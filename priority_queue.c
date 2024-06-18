#include "../project 4/priority_queue.h"

void printArray(priority_queue *q) {
    for (int i = 0; i < q->size; ++i) {
        printf("%c: %llu\n", q->data[i]->letter, q->data[i]->frequency);
    }
    printf("\n");
}

void reallocateMemory(priority_queue *q, enum ReallocateType type) {
    if (type == INCREASE) {
        if (q->capacity == 0) {
            q->capacity = 1;
        } else {
            q->capacity *= 2;
        }
        node **tmp = (node **) (malloc(sizeof(node *) * q->capacity));
        for (uint64_t i = 0; i < q->size; ++i) {
            tmp[i] = q->data[i];
        }
        free(q->data);
        q->data = tmp;
    } else {
        if (q->capacity == 1) {
            q->capacity = 0;
            free(q->data);
            q->data = NULL;
        } else {
            q->capacity /= 2;
            node **tmp = (node **) (malloc(sizeof(node *) * q->capacity));
            for (uint64_t i = 0; i < q->size; ++i) {
                tmp[i] = q->data[i];
            }
            free(q->data);
            q->data = tmp;
        }
    }
}

priority_queue *createQueue() {
    priority_queue *q = (priority_queue *) (malloc(sizeof(priority_queue)));
    q->data = NULL;
    q->capacity = 0;
    q->size = 0;
    return q;
}

void heapify(priority_queue *q, int64_t i) {
    if (q->size != 1) {
        int64_t largest = i;
        int64_t l = 2 * i + 1;
        int64_t r = 2 * i + 2;
        if (l < q->size && q->data[l]->frequency < q->data[largest]->frequency) {
            largest = l;
        }
        if (r < q->size && q->data[r]->frequency < q->data[largest]->frequency) {
            largest = r;
        }

        if (largest != i) {
            swap(q->data[i], q->data[largest]);
            heapify(q, largest);
        }
    }
}

void insert(priority_queue *q, node *newNum) {
    if (q->size + 1 > q->capacity) {
        reallocateMemory(q, INCREASE);
    }

    if (q->size == 0) {
        q->data[0] = newNum;
        q->size += 1;
    } else {
        q->data[q->size] = newNum;
        q->size += 1;
        for (int64_t i = (int64_t) (q->size) / 2 - 1; i >= 0; i--) {
            heapify(q, i);
        }
    }
}

void deleteRoot(priority_queue *q, char letter) {
    int64_t i = 0;
    for (; i < q->size; i++) {
        if (letter == q->data[i]->letter)
            break;
    }

    swap(q->data[i], q->data[q->size - 1]);
    q->size -= 1;
    for (i = (int64_t) (q->size) / 2 - 1; i >= 0; i--) {
        heapify(q, i);
    }
}

void deallocateQueue(priority_queue *q) {
    for (uint64_t i = 0; i < q->size; ++i) {
        free(q->data[i]);
    }
    free(q->data);
}
