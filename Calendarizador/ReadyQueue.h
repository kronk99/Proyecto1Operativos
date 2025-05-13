#ifndef READY_QUEUE_H
#define READY_QUEUE_H

#include "../Threads/Car.h"

typedef struct Node {
    Car *car;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    Node *tail; //anadido luis , facilita las cosas en RR
    Node *current; // nuevo: para recorrido sin modificar la cola
} ReadyQueue;

void init_queue(ReadyQueue *q);
Car* dequeue(ReadyQueue *q);
int is_empty(ReadyQueue *q);

// Avanza en la cola sin modificarla, devuelve NULL al final
Car* queue_next(ReadyQueue *q);

// Reinicia el recorrido (current = head)
void queue_reset(ReadyQueue *q);


#endif


//Administra la cola de vehículos listos para cruzar según el algoritmo del usuario