#ifndef TESTING_H
#define TESTING_H

#include "Threads/Car.h"
#include "Calendarizador/ReadyQueue.h"
#include "Calendarizador/Scheduler.h"
typedef struct {
        ReadyQueue *queueRight;
        ReadyQueue *queueLeft;
    } FlujoArgs;
//void ejecutarSimulacion(Car** carros, int cantidadCarros, const char* tipoCalendarizador);
int car_function(void *arg) ;
//void initQueue(int tipoCalendarizador);
void createCars(int deportivos, int ambulancias, int normales, 
                SDL_Texture* carSport, SDL_Texture* carAmbulance, SDL_Texture* carNormal,
                ReadyQueue* queueRight, ReadyQueue* queueLeft);
int flujo_carritos(void* arg);     
#endif
