#ifndef CAR_H
#define CAR_H

#include <time.h>
#include "CEmutex.h"
#include <SDL2/SDL_render.h>


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
    SDL_Texture* carTexture; //puntero a la textura
    CEmutex_t *mutex; //Mutex propio del carro para impedir que ejecute
    //recuerde que la funcion del hilo lee este mutex, no el mutex de la pista, 
    //cada hilo tiene su propio mutex!!!!!!
} Car;
#endif
