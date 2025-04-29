#include <stdlib.h>
#include "ReadyQueue.h"

void init_queue(ReadyQueue *q) {
    q->head = NULL;
}

void enqueue_priority(ReadyQueue *q, Car *car) {
    Node *new_node = malloc(sizeof(Node));
    new_node->car = car;
    new_node->next = NULL;

    if (!q->head || car->priority < q->head->car->priority) {
        new_node->next = q->head;
        q->head = new_node;
        return;
    }

    Node *current = q->head;
    while (current->next && car->priority >= current->next->car->priority) {
        current = current->next;
    }

    new_node->next = current->next;
    current->next = new_node;
}

Car *dequeue(ReadyQueue *q) {
    if (!q->head) return NULL;
    Node *node = q->head;
    q->head = node->next;
    Car *car = node->car;
    free(node);
    return car;
}

int is_empty(ReadyQueue *q) {
    return q->head == NULL;
}
