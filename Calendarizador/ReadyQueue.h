#ifndef READY_QUEUE_H
#define READY_QUEUE_H

#include "../CEthreads/include/Car.h"

typedef struct Node {
    Car *car;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    Node *tail; //anadido luis , facilita las cosas en RR
} ReadyQueue;

void init_queue(ReadyQueue *q);
Car* dequeue(ReadyQueue *q);
int is_empty(ReadyQueue *q);

#endif


//Administra la cola de vehículos listos para cruzar según el algoritmo del usuario