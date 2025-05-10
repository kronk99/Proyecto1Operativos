#ifndef TESTING_H
#define TESTING_H

#include "CEthreads/include/Car.h"
void ejecutarSimulacion(Car** carros, int cantidadCarros, const char* tipoCalendarizador);
void *car_function(void *arg) ;
void initQueue(int tipoCalendarizador);
void createCars(int deportivos, int ambulancias, int normales, SDL_Texture* carSport,SDL_Texture* carAmbulance,SDL_Texture* carNormal);
#endif
