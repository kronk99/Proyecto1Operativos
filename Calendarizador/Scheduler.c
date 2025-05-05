#include "Scheduler.h"
#include "algoritmos/prioridad.h"
#include "algoritmos/SJF.h"


//acomoda la ready q en base al calendarizador establecido
Algoritmo algoritmo_actual = ALG_FCFS;
ReadyQueue global_queue; //definicion global de la variable , Lista derecha
ReadyQueue global_queueLeft; //definicion global de la variable , Lista izquierda.

void seleccionar_algoritmo(int seleccion) {
    algoritmo_actual = (Algoritmo)seleccion;
    init_queue(&global_queue); //inicializa la cola derecha
    init_queue(&global_queueLeft); //inicializa la cola izquierda
    /*
    if (algoritmo_actual ==ALG_RR){
        initRR(rrScheduler); //inicializa el puntero del round robin si se selecciono round robin
    }*/
}
//hay que trabajar esto, hacerlo para una u otra cola dependiendo del parametro
//1 si es derecha, 0 si es cola izquierda.
void encolar_con_algoritmo(Car *car,int queue) {
    switch (algoritmo_actual) {
        case ALG_PRIORIDAD:
            if(queue == 1){
                enqueue_priority(&global_queue, car);
            }
            else{
                enqueue_priority(&global_queueLeft, car);
            }
            break;
        case ALG_SJF:
            if(queue == 1){
                enqueue_SJF(&global_queue, car);
            }
            else{
                enqueue_SJF(&global_queueLeft, car);
            }
            enqueue_SJF(&global_queue, car);
            break;
        case ALG_FCFS:
            break;
        case ALG_RR:
            break;
            //aca debo de hacer una condicion adicional , por que si ya agrego los carros
            //recuerde que debo dehacer re-schedule cada que cambio el quantum 
            //scheduleCar(rrScheduler,car); //anade carro a la lista circular
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
