#include "Scheduler.h"
#include "algoritmos/prioridad.h"
#include "algoritmos/SJF.h"
#include "algoritmos/RoundRobin.hilos"

//acomoda la ready q en base al calendarizador establecido
Algoritmo algoritmo_actual = ALG_FCFS;
ReadyQueue global_queue;

void seleccionar_algoritmo(int seleccion) {
    algoritmo_actual = (Algoritmo)seleccion;
    init_queue(&global_queue);
}

void encolar_con_algoritmo(Car *car) {
    switch (algoritmo_actual) {
        case ALG_PRIORIDAD:
            enqueue_priority(&global_queue, car);
            break;
        case ALG_SJF:
            enqueue_SJF(&global_queue, car);
            break;
        case ALG_RR:
            
            break;
        default:
            // futura implementación
            break;
    }
}

Car* siguiente_carro() {
    return dequeue(&global_queue);
}
