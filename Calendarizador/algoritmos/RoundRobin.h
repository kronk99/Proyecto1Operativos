#ifndef RoundRobin
#define RoundRobin
#include "Car.h"
#include "ReadyQueue.h"
typedef struct roundRobin{
    int Qos;
    Node *head;
    Node *tail;
    Node*current;
}roundRobin;
//esto es el nodo


void initRR(roundRobin *circular); //iniliciazdor del RR
void scheduleCar(roundRobin *circular,Car *car); //inserta el carro a la la lista circular
void checkQuantum(roundRobin *roundRobin,int quantum); //hace el context switch 
Car* getcurrentCar(); //
void decreaseQuantum(roundRobin *roundRobin);
void schedulereadyCar(ReadyQueue *readyqueue,roundRobin *roundRobin); 
#endif