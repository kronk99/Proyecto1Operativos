#include "SJF.h"
#include <stdlib.h>

void enqueue_SJF(ReadyQueue *q, Car *car) {
    Node *new_node = malloc(sizeof(Node));
    new_node->car = car;
    new_node->next = NULL;

    if (!q->head || car->burstTime < q->head->car->burstTime) {
        new_node->next = q->head;
        q->head = new_node;
        return;
    }

    Node *current = q->head;
    while (current->next && car->burstTime >= current->next->car->burstTime) {
        current = current->next;
    }

    new_node->next = current->next;
    current->next = new_node;
}
