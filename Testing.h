#ifndef TESTING_H
#define TESTING_H

#include "Threads/Car.h"
#include "Calendarizador/ReadyQueue.h"
#include "Calendarizador/Scheduler.h"

//void ejecutarSimulacion(Car** carros, int cantidadCarros, const char* tipoCalendarizador);
void *car_function(void *arg) ;
//void initQueue(int tipoCalendarizador);
void createCars(int deportivos, int ambulancias, int normales, 
                SDL_Texture* carSport, SDL_Texture* carAmbulance, SDL_Texture* carNormal,
                ReadyQueue* queueRight, ReadyQueue* queueLeft);
#endif
