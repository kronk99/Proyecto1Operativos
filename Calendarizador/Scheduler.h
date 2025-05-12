#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "../Threads/Car.h"
#include "ReadyQueue.h"
#include "algoritmos/RoundRobin.h"

typedef enum {
    ALG_FCFS,
    ALG_SJF,
    ALG_PRIORIDAD,
    ALG_RR
} Algoritmo;

extern Algoritmo algoritmo_actual;
extern ReadyQueue global_queue;
extern ReadyQueue global_queueLeft;
extern roundRobin* rrScheduler;

void seleccionar_algoritmo(int seleccion, ReadyQueue *rightQueue, ReadyQueue *leftQueue);
void encolar_con_algoritmo(Car *car, ReadyQueue *queue);
Car* siguiente_carro(ReadyQueue *queue);

#endif
