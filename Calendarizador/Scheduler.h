#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Car.h"
#include "ReadyQueue.h"

typedef enum {
    ALG_FCFS,
    ALG_SJF,
    ALG_PRIORIDAD
    ALG_RR
} Algoritmo;

extern Algoritmo algoritmo_actual;
extern ReadyQueue global_queue; //esto es como una instancia global

void seleccionar_algoritmo(int seleccion);
void encolar_con_algoritmo(Car *car);
Car* siguiente_carro(void);

#endif
