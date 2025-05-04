#ifndef RoundRobin
#define RoundRobin
#include "../ReadyQueue.h"

void initRR(roundRobin *circular,Car *car);
//insertar un carro a la lista de calendarizacion de RR (circular)
void scheduleCar(roundRobin *circular,Car *car);
void checkQuantum(roundRobin *circular, int quantum);
Node* getcurrent(roundRobin *circular);
void decreaseQuantum(roundRobin *circular);
void nextcurrent(roundRobin *circular);
void schedulereadyCar(ReadyQueue *readyqueue,roundRobin *circular);

#endif