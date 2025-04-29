#ifndef READY_QUEUE_H
#define READY_QUEUE_H

#include "Car.h"

typedef struct Node {
    Car *car;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
} ReadyQueue;

// funciones
void init_queue(ReadyQueue *q);
void enqueue_priority(ReadyQueue *q, Car *car);
Car *dequeue(ReadyQueue *q);
int is_empty(ReadyQueue *q);

#endif

//Administra la cola de vehículos listos para cruzar según el algoritmo del usuario