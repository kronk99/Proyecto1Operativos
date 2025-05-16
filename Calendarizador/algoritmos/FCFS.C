#include "FCFS.h"
#include <stdlib.h>
void enqueue(ReadyQueue *q, Car *car) {
    Node *new_node = malloc(sizeof(Node));
    new_node->car = car;
    new_node->next = NULL;

    if (!q->head) {
        
        q->head = new_node;
    } else {
        Node *current = q->head;
        while (current->next) {
            current = current->next;
        }
        current->next = new_node;
    }
}