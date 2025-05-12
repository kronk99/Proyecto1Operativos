#ifndef FLOW_H
#define FLOW_H

#include "../Threads/Car.h"
#include "../Calendarizador/ReadyQueue.h"
#include "../Calendarizador/Scheduler.h"
typedef struct {
    atomic_int timeCount; // posición actual en la carretera , posicion x
    atomic_int dirLetrero; //0 si es izquierda , 1 si es derecha
    int time_Value ;  //valor que asigna en makefile de cuando hacer cambios de tiempo
} Timer;
void equity(int w, ReadyQueue* quequeRight, ReadyQueue* quequeLeft,SDL_Texture* carSport, SDL_Texture* carNormal, SDL_Texture* carEmergency);
void draw_car(int tipo, int direccion, SDL_Texture* imagen_carro);
int contador(void *arg);
void letrero( ReadyQueue* queueRight, ReadyQueue* queueLeft, Timer *tiempo);
//void sing(struct CarList* left, struct CarList* right, int interval);
//void fifo(struct CarList* left, struct CarList* right);

#endif