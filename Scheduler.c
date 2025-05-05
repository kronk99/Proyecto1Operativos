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
    if (algoritmo_actual ==ALG_RR){
        initRR(rrScheduler); //inicializa el puntero del round robin si se selecciono round robin
    }
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
        case ALG_FCFS:
        case ALG_RR:
            //aca debo de hacer una condicion adicional , por que si ya agrego los carros
            //recuerde que debo dehacer re-schedule cada que cambio el quantum 
            scheduleCar(rrScheduler,car); //anade carro a la lista circular
            //schedulereadyCar(&global_queue,rrScheduler);

            break;

        default:
            // futura implementación
            break;
    }
}

Car* siguiente_carro() { //cambios para el RR, en vez de usar la cola del dequeue , voy a
    //usar la cola de RR como cola de listo debido al quantum.
    return dequeue(&global_queue);
}
