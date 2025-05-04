#ifndef TESTING_H
#define TESTING_H

#include "CEthreads/include/Car.h"
void ejecutarSimulacion(Car** carros, int cantidadCarros, const char* tipoCalendarizador);
void *car_function(void *arg) ;

#endif
