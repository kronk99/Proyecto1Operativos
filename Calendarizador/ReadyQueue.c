#include "ReadyQueue.h"
#include <stdlib.h>

void init_queue(ReadyQueue *q) {
    q->head = NULL;
    q->current = NULL;
}

Car* dequeue(ReadyQueue *q) {
    if (!q->head) return NULL;
    Node *temp = q->head;
    q->head = q->head->next;
    Car *car = temp->car;
    free(temp);
    return car;
}

Car* queue_next(ReadyQueue *q) {
    if (!q->current) return NULL;
    Car *car = q->current->car;
    q->current = q->current->next;
    return car;
}

void queue_reset(ReadyQueue *q) {
    q->current = q->head;
}



//hacer un metodo next para ir pintando en interfaz.

int is_empty(ReadyQueue *q) {
    return q->head == NULL;
}
