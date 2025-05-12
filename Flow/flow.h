#ifndef FLOW_H
#define FLOW_H

#include "../Threads/Car.h"
#include "../Calendarizador/ReadyQueue.h"
#include "../Calendarizador/Scheduler.h"

void equity(int w, ReadyQueue* quequeRight, ReadyQueue* quequeLeft);
//void sing(struct CarList* left, struct CarList* right, int interval);
//void fifo(struct CarList* left, struct CarList* right);

#endif