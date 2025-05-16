#ifndef QUEUE_H
#define QUEUE_H
#include "../ReadyQueue.h"
#include "../Threads/Car.h"
#include <stdlib.h>

// Funciones de la cola
void enqueue(ReadyQueue *q, Car *car);

#endif // QUEUE_H