#include "ReadyQueue.h"
#include <stdlib.h>

void init_queue(ReadyQueue *q) {
    q->head = NULL;
}

Car* dequeue(ReadyQueue *q) {
    if (!q->head) return NULL;
    Node *temp = q->head;
    q->head = q->head->next;
    Car *car = temp->car;
    free(temp);
    return car;
}

int is_empty(ReadyQueue *q) {
    return q->head == NULL;
}
