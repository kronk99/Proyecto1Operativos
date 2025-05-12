#ifndef CAR_H
#define CAR_H
#include <time.h>
#include "CEthread.h"
#include <SDL2/SDL_render.h>

typedef struct {
    int id;            // identificador unico del carro
    atomic_int positionX; // posición actual en la carretera , posicion x
    atomic_int hasArrived; //bandera para indicar que el hilo ha llegado
    int posy; //el valor y no ocupa ser atomico
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