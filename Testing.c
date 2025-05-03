#include "CEthreads.h"
#include "CEmutex.h"
#include "Car.h"
#include "ReadyQueue.h"
#include "Calendarizador/Scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

///*
#define NUM_CARROS 5

void *car_function(void *arg) {
    Car *car = (Car *)arg;
    CEmutex_t *mutex = car->mutex;

    printf("Carro #%d está esperando el mutex para cruzar...\n", car->id);
    CEmutex_lock(mutex);

    printf("Carro #%d (tipo %d, prioridad %d) está cruzando (mutex adquirido)\n", car->id, car->type, car->priority);
    sleep(car->burstTime);
    printf("Carro #%d ha cruzado (mutex liberado)\n", car->id);

    CEmutex_unlock(mutex);

    free(car);
    return NULL;
}

int main() {
    srand(time(NULL));

    // inicializar mutex
    CEmutex_t mutex;
    CEmutex_init(&mutex);

    // preguntar por el algoritmo
    int seleccion;
    printf("Seleccione el algoritmo de calendarización con MUTEX:\n");
    printf("0 = FCFS\n1 = SJF\n2 = Prioridad\n> ");
    scanf("%d", &seleccion);
    seleccionar_algoritmo(seleccion);

    // crear y encolar los carros
    for (int i = 0; i < NUM_CARROS; i++) {
        Car *car = malloc(sizeof(Car));
        car->id = i + 1;
        car->type = rand() % 3;
        car->speed = car->type + 1;
        car->burstTime = 4 - car->speed;
        car->priority = rand() % 5;
        car->position = 0;
        car->direction = rand() % 2;
        clock_gettime(CLOCK_REALTIME, &car->arrival_time);

        // PASO CLAVE: pasar el puntero al mutex al carro
        car->mutex = &mutex;

        encolar_con_algoritmo(car);
    }

    // lanzar hilos de acuerdo al algoritmo
    while (!is_empty(&global_queue)) {
        Car *car = siguiente_carro();
        CEthread_t thread;

        printf("Lanzando carro #%d\n", car->id);
        int tid = CEthread_create(car_function, car);
        thread.tid = tid;

        printf("Esperando a que carro #%d termine\n", car->id);
        void *retval;
        CEthread_join(thread, &retval);
        printf("Carro #%d terminó correctamente\n", car->id);
    }

    puts("Todos los carros han cruzado.");

    // destruir mutex
    CEmutex_destroy(&mutex);

    return 0;
}

//*/
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
*/

// PPRUEBAAAA PRUEBITA: 
//se inicializa un mutex.
//screa un hilo que:
    //toma el mutex
    //imprime 
    //libera el mutex
//el main espera que el hilo termine.
//se destruye el mutex.
