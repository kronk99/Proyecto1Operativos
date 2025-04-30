#include "prioridad.h"
#include <stdlib.h>

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
