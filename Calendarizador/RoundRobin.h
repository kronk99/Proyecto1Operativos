#ifndef RoundRobin
#define RoundRobin
#include "CEthreads/include/Car.h"
#include "Calendarizador/ReadyQueue.h"
typedef struct roundRobin{
    int Qos;
    Node *head;
    Node *tail;
    Node *current;
}roundRobin;
//esto es el nodo
typedef struct Node{
    Car *Car;
    struct Node  *next;

}Node;
void initRR(roundRobin *circular,Car *car); //iniliciazdor del RR
void scheduleCar(roundRobin *circular,Car *car); //inserta el carro a la la lista circular
int checkQuantum(roundRobin *roundRobin); //hace el context switch 
Car* getcurrentCar(); //
void decreaseQuantum(roundRobin *roundRobin);
void schedulereadyCar(ReadyQueue *readyqueue,Car *car); 
#endif