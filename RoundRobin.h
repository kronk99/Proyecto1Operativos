#ifndef RoundRobin
#define RoundRobin
#include "CEthreads/include/Car.h"
#include "Calendarizador/ReadyQueue.h"
typedef struct roundRobin{
    int Qos;
    Node *head;
    Node *tail;
    Node*current;
}roundRobin;
//esto es el nodo
typedef struct Node{
    Car *Car;
    struct Node  *next;

}Node;

void initRR(roundRobin *circular); //iniliciazdor del RR
void scheduleCar(roundRobin *circular,Car *car); //inserta el carro a la la lista circular
void checkQuantum(roundRobin *roundRobin,int quantum); //hace el context switch 
Car* getcurrentCar(); //
void decreaseQuantum(roundRobin *roundRobin);
void schedulereadyCar(ReadyQueue *readyqueue,roundRobin *roundRobin); 
#endif