#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "../CEthreads/include/Car.h"
#include "ReadyQueue.h"
#include "../Calendarizador/algoritmos/RoundRobin.h"

typedef enum {
    ALG_FCFS,
    ALG_SJF,
    ALG_PRIORIDAD,
    ALG_RR
} Algoritmo;

extern Algoritmo algoritmo_actual;
extern ReadyQueue global_queue;
extern ReadyQueue global_queueLeft;
//extern roundRobin* rrScheduler;

void seleccionar_algoritmo(int seleccion);
void encolar_con_algoritmo(Car *car,int queue);
Car* siguiente_carro(void);

#endif
