#include "Threads/CEthread.h"
#include "Threads/Car.h"
#include "Calendarizador/ReadyQueue.h"
#include "Calendarizador/Scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "Testing.h"
#include "Threads/CEthread.h"
#include "interfaz.h"
#include "Flow/flow.h"

#define STACK_SIZE (1024 * 1024)  // 1 MB pila
//En este archivo estara lo necesario para utilizar en el main
//esta es la funcion de comportamiento del carro, aca se llama a sdl para realizar el movimiento
//y actualizar la pantalla para pintar, llamar a interfaz con cada actualizacion de movimiento

int car_function(void *arg) {
    Car *car = (Car *)arg;
    CEmutex_t *mutex = car->mutex;   
    //CEmutex_lock(mutex); //espera hasta que le haga un unlock en otro lado
    CEmutex_lock(car->mutex); //espera agarrar el mutex
    printf("Carro #%d se le hizo unlock al mutex...\n", car->id);

    draw_car(car->type, car->direction, car->carTexture);

    atomic_store(&car->hasArrived, 1);

    //Recordar liberar la memoria del carro 


    printf("Carro #%d ha cruzado (mutex liberado)\n", car->id);
    CEmutex_unlock(car->mutex);

    //CEthread_end();
    //CEmutex_unlock(mutex);
    //pthread_exit(NULL);  // termina el hilo correctamente
  // mata el hilo (nunca retorna)
    //deberia de destuir el hilo
    //aca hace falta un metodo de pintado global ()paintall
    //free(car); el free lo hace en el manejador de direccion
    return NULL;
}



void createCars(int deportivos, int ambulancias, int normales, 
                SDL_Texture* carSport, SDL_Texture* carAmbulance, SDL_Texture* carNormal,
                ReadyQueue* queueRight, ReadyQueue* queueLeft) {

    CEmutex_t *car_mutex; // mutex para el carro
    int id = 0;

    for (int j = 0; j < deportivos; j++) {
        
        Car* car = malloc(sizeof(Car));
        car_mutex = malloc(sizeof(CEmutex_t));
        CEmutex_init(car_mutex);
        CEmutex_lock(car_mutex);
        car->mutex = car_mutex;
        //carposx = x
        //carposy = y
        car->id = id++;
        car->type = 1;
        car->direction = 1; // derecha a izquierda
        car->carTexture = carSport;
        atomic_init(&car->hasArrived, 0);


        CEthreads_t car_thread;

        car_thread.start_routine = car_function;
        car_thread.arg = car;
        car_thread.stack_size = STACK_SIZE;
        car_thread.is_finished = 0;

        if (CEthread_create(&car_thread) != 0) {
            printf(stderr, "Error creando hilo\n");
            printf("error al crear hilo");
        }
        //Ethread_join(thread, &retval);

        encolar_con_algoritmo(car, queueRight);
    }

    for (int j = 0; j < deportivos; j++) {
        Car* car = malloc(sizeof(Car));
        car_mutex = malloc(sizeof(CEmutex_t));
        CEmutex_init(car_mutex);
        CEmutex_lock(car_mutex);
        car->mutex = car_mutex;
        car->id = id++;
        car->type = 1;
        car->direction = 0; // izquierda a derecha
        car->carTexture = carSport;
        atomic_init(&car->hasArrived, 0);


        CEthreads_t car_thread;

        car_thread.start_routine = car_function;
        car_thread.arg = car;
        car_thread.stack_size = STACK_SIZE;
        car_thread.is_finished = 0;

        if (CEthread_create(&car_thread) != 0) {
            printf(stderr, "Error creando hilo\n");
            //fprint("error al crear hilo");
        }

        encolar_con_algoritmo(car, queueLeft);
    }

    for (int j = 0; j < normales; j++) {
        Car* car = malloc(sizeof(Car));
        car_mutex = malloc(sizeof(CEmutex_t));
        CEmutex_init(car_mutex);
        CEmutex_lock(car_mutex);
        car->mutex = car_mutex;
        car->id = id++;
        car->type = 0;
        car->direction = 1; // derecha a izquierda
        car->carTexture = carNormal;
        atomic_init(&car->hasArrived, 0);


        CEthreads_t car_thread;

        car_thread.start_routine = car_function;
        car_thread.arg = car;
        car_thread.stack_size = STACK_SIZE;
        car_thread.is_finished = 0;

        if (CEthread_create(&car_thread) != 0) {
            printf(stderr, "Error creando hilo\n");
            //fprint("error al crear hilo");
        }

        encolar_con_algoritmo(car, queueRight);
    }

    for (int j = 0; j < normales; j++) {
        Car* car = malloc(sizeof(Car));
        car_mutex = malloc(sizeof(CEmutex_t));
        CEmutex_init(car_mutex);
        CEmutex_lock(car_mutex);
        car->mutex = car_mutex;
        car->id = id++;
        car->type = 0;
        car->direction = 0; // izquierda a derecha
        car->carTexture = carNormal;
        atomic_init(&car->hasArrived, 0);


        CEthreads_t car_thread;

        car_thread.start_routine = car_function;
        car_thread.arg = car;
        car_thread.stack_size = STACK_SIZE;
        car_thread.is_finished = 0;

        if (CEthread_create(&car_thread) != 0) {
            printf(stderr, "Error creando hilo\n");
            //fprint("error al crear hilo");
        }

        encolar_con_algoritmo(car, queueLeft);
    }

    for (int j = 0; j < ambulancias; j++) {
        Car* car = malloc(sizeof(Car));
        car_mutex = malloc(sizeof(CEmutex_t));
        CEmutex_init(car_mutex);
        CEmutex_lock(car_mutex);
        car->mutex = car_mutex;
        car->id = id++;
        car->type = 2;
        car->direction = 1; // derecha a izquierda
        car->carTexture = carAmbulance;
        atomic_init(&car->hasArrived, 0);


        CEthreads_t car_thread;

        car_thread.start_routine = car_function;
        car_thread.arg = car;
        car_thread.stack_size = STACK_SIZE;
        car_thread.is_finished = 0;

        if (CEthread_create(&car_thread) != 0) {
            printf(stderr, "Error creando hilo\n");
            printf("error al crear hilo");
        }

        encolar_con_algoritmo(car, queueRight);
    }

    for (int j = 0; j < ambulancias; j++) {
        Car* car = malloc(sizeof(Car));
        car_mutex = malloc(sizeof(CEmutex_t));
        CEmutex_init(car_mutex);
        CEmutex_lock(car_mutex);
        car->mutex = car_mutex;
        car->id = id++;
        car->type = 2;
        car->direction = 0; // izquierda a derecha
        car->carTexture = carAmbulance;
        atomic_init(&car->hasArrived, 0);


        CEthreads_t car_thread;

        car_thread.start_routine = car_function;
        car_thread.arg = car;
        car_thread.stack_size = STACK_SIZE;
        car_thread.is_finished = 0;

        if (CEthread_create(&car_thread) != 0) {
            printf(stderr, "Error creando hilo\n");
            printf("error al crear hilo");
        }

        encolar_con_algoritmo(car, queueLeft);
    }
}

/*
void initQueue(int tipoCalendarizador){
    seleccionar_algoritmo(tipoCalendarizador); //inicializa las colas, metodo de calendarizador
     
    //el 3 es el calendarizado de mariana
}*/
//dequeue


//#define NUM_CARROS 




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
