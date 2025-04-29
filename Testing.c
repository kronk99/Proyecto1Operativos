#include "CEthreads.h"
#include "CEmutex.h"
#include "Car.h"
#include "ReadyQueue.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


#define NUM_CARROS 5


void *car_function(void *arg) {
    Car *car = (Car *)arg;
    printf("Carro #%d (tipo %d, prioridad %d) quiere cruzar\n", car->id, car->type, car->priority);
    sleep(car->burstTime);  // Simula el cruce
    printf("Carro #%d ha cruzado\n", car->id);
    free(car);  // Libera memoria
    return NULL;
}

int main() {
    srand(time(NULL));

    ReadyQueue queue;
    init_queue(&queue);

    for (int i = 0; i < NUM_CARROS; i++) {
        Car *car = malloc(sizeof(Car));
        car->id = i + 1;
        car->type = rand() % 3;  // 0: normal, 1: deportivo, 2: emergencia
        car->speed = car->type + 1;
        car->burstTime = 4 - car->speed;
        car->priority = 2 - car->type;
        car->position = 0;
        car->direction = rand() % 2;
        clock_gettime(CLOCK_REALTIME, &car->arrival_time);

        enqueue_priority(&queue, car);
    }

    while (!is_empty(&queue)) {
        Car *car = dequeue(&queue);
        CEthread_t thread;
    
        printf("Lanzando carro #%d\n", car->id);
        int tid = CEthread_create(car_function, car);
        thread.tid = tid;
    
        printf("Esperando a que carro #%d termine\n", car->id);
        void *retval;
        CEthread_join(thread, &retval);
        printf("Carro #%d terminó correctamente\n", car->id);
    }
    

    return 0;
}

/*
void *my_start_routine(void *arg) {
    // Implementación para el hilo
    CEmutex_t *mutex = (CEmutex_t *)arg;
    CEmutex_lock(mutex);
    printf("ahora SIIIIII???? afuera\n");
    CEmutex_unlock(mutex);
    return NULL;
}

int main() {
    CEmutex_t mutex;
    CEmutex_init(&mutex);
    CEthread_t thread;
    int tid = CEthread_create(my_start_routine, &mutex);
    thread.tid = tid;
    void *retval;
    CEthread_join(thread, &retval);
    CEmutex_destroy(&mutex);
    return 0;
}

// PPRUEBAAAA PRUEBITA: 
//se inicializa un mutex.
//screa un hilo que:
    //toma el mutex
    //imprime 
    //libera el mutex
//el main espera que el hilo termine.
//se destruye el mutex.
*/