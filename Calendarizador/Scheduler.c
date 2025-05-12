#include "Scheduler.h"
#include "ReadyQueue.h"
#include "algoritmos/prioridad.h"
#include "algoritmos/SJF.h"

// Variable para guardar el algoritmo seleccionado (mantener global o pasar también si quieres total desacoplamiento)
Algoritmo algoritmo_actual = ALG_FCFS;

void seleccionar_algoritmo(int seleccion, ReadyQueue *rightQueue, ReadyQueue *leftQueue) {
    algoritmo_actual = (Algoritmo)seleccion;
    init_queue(rightQueue);     // inicializa la cola derecha
    init_queue(leftQueue);      // inicializa la cola izquierda
    /*
    if (algoritmo_actual == ALG_RR) {
        initRR(rrScheduler); // inicializa el puntero del round robin si se seleccionó round robin
    }*/
}

// Encola un carro en la cola que se le pasa como parámetro
void encolar_con_algoritmo(Car *car, ReadyQueue *queue) {
    switch (algoritmo_actual) {
        case ALG_PRIORIDAD:
            enqueue_priority(queue, car);
            break;
        case ALG_SJF:
            enqueue_SJF(queue, car);
            break;
        case ALG_FCFS:
            enqueue(queue, car); // Asumo que existe `enqueue` estándar para FCFS
            break;
        case ALG_RR:
            // scheduleCar(rrScheduler, car);
            // schedulereadyCar(queue, rrScheduler);
            break;
        default:
            // futura implementación
            break;
    }
}

// Devuelve el siguiente carro según la cola que se le pase
Car* siguiente_carro(ReadyQueue *queue) {
    return dequeue(queue);
}
