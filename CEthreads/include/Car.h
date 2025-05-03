#ifndef CAR_H
#define CAR_H

#include <time.h>
#include "CEmutex.h"


typedef struct {
    int id;            // identificador unico del carro
    int position;      // posición actual en la carretera
    int speed;         // velocidad (puede derivarse del tipo)
    int priority;      // prioridad para el algoritmo de prioridad
    int burstTime;     // tiempo estimado para cruzar (SJF)
    int type;          // 0: normal, 1: deportivo, 2: emergencia
    int deadline;      // si aplica, para tiempo real
    int direction;     // 0: izquierda a derecha, 1: derecha a izquierda
    struct timespec arrival_time; // momento de llegada (para FCFS)
    CEmutex_t *mutex; 
} Car;
#endif
