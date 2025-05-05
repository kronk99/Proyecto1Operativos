#include "CEthreads.h"
#include "CEmutex.h"
#include "Car.h"
#include "ReadyQueue.h"
#include "Scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <time.h>
#include <string.h>
#include "Testing.h"

//En este archivo estara lo necesario para utilizar en el main
//esta es la funcion de comportamiento del carro, aca se llama a sdl para realizar el movimiento
//y actualizar la pantalla para pintar, llamar a interfaz con cada actualizacion de movimiento

void *car_function(void *arg) {
    Car *car = (Car *)arg;
    CEmutex_t *mutex = car->mutex;

    printf("Carro #%d está esperando el mutex para cruzar...\n", car->id);
    CEmutex_lock(mutex); //espera hasta que le haga un unlock en otro lado
    //es decir la funcion que va a descalendarizar los carros

    printf("Carro #%d (tipo %d, prioridad %d) está cruzando (mutex adquirido)\n", car->id, car->type, car->priority);
    sleep(car->burstTime);
    printf("Carro #%d ha cruzado (mutex liberado)\n", car->id);

    CEmutex_unlock(mutex);

    free(car);
    return NULL;
}
//Archivo de configuracion del juego
void ejecutarSimulacion(Car** carros, int cantidadCarros, const char* tipoCalendarizador) {
    srand(time(NULL));
    CEmutex_t mutex;
    CEmutex_init(&mutex);

    if (strcmp(tipoCalendarizador, "FCFS") == 0) seleccionar_algoritmo(0);
    else if (strcmp(tipoCalendarizador, "SJF") == 0) seleccionar_algoritmo(1);
    else if (strcmp(tipoCalendarizador, "Prioridad") == 0) seleccionar_algoritmo(2);
    else seleccionar_algoritmo(0);  // por defecto

    for (int i = 0; i < cantidadCarros; i++) {
        carros[i]->speed = carros[i]->type + 1;
        carros[i]->burstTime = 4 - carros[i]->speed;
        carros[i]->priority = rand() % 5;
        carros[i]->position = 0;
        clock_gettime(CLOCK_REALTIME, &carros[i]->arrival_time);
        carros[i]->mutex = &mutex;
        encolar_con_algoritmo(carros[i]);
    }

    while (!is_empty(&global_queue)) {
        Car* car = siguiente_carro();
        CEthread_t thread;
        printf("Lanzando carro #%d\n", car->id);
        int tid = CEthread_create(car_function, car);
        thread.tid = tid;
        void* retval;
        CEthread_join(thread, &retval);
        printf("Carro #%d terminó correctamente\n", car->id);
    }

    puts("Todos los carros han cruzado.");
    CEmutex_destroy(&mutex);
}
//metodo para crear los carros en base al input , existiran entonces 2 filas de listo en scheduler
//esto va a ocupar un parametro extra para indicar a cual de las 2 colas insertar
//esto es para la insercion predefinida, para la insercion en excecute es diferente btw
void createCars(int deportivos, int ambulancias, int normales, SDL_Texture* carSport,SDL_Texture* carAmbulance,SDL_Texture* carNormal){
    //esto falta de perfeccionar, dado que a carro le hace falta mas datos para encolar
    //como porejemplo que las ambulancias tienen una prioridad , entonces se deberian 
    //de encolar primero independientemente del algoritmo
    // === CREAR LISTA DE CARROS ===
    //init_queue(&global_queue);
    //init_queue(&global_queueLeft);
    //llamo de una vez al algoritmo de calendarizado 
    CEmutex_t *car_mutex; //mutex del carro, para bloquear el hilo.
    int id=0;
    for (int j = 0; j < deportivos; j++) {
        //al carro hay que anadirle la textura sdl
        Car* car = malloc(sizeof(Car));
        car_mutex= malloc(sizeof(CEmutex_t)); //crea el mutex
        CEmutex_init(car_mutex); //inicia el mutex
        CEmutex_lock(car_mutex); //bloquea el mutex , asi al crear el hilo no 
        //se va a ejecutar de una
        car->mutex =car_mutex;
        car->id = id++;
        car->type = 1;
        car->direction = 1; // 0: izquierda a derecha, 1: derecha a izquierda
        car->carTexture = carSport;
        //genera el hilo
        CEthread_t thread;
        int tid = CEthread_create(car_function, car);
        thread.tid = tid;
        void *retval;
        CEthread_join(thread, &retval);
        //end generar hilo, esta dormido ya que inicio con el mutex tomado
        //se necesita un algoritmo descolador que le haga pop a la cola, y haga
        //car ->unlock mutex.
        encolar_con_algoritmo(car,1); //lo encola

    }
    for (int j = 0; j < deportivos; j++) {
        //al carro hay que anadirle la textura sdl
        Car* car = malloc(sizeof(Car));
        car_mutex= malloc(sizeof(CEmutex_t)); //crea el mutex
        CEmutex_init(car_mutex); //inicia el mutex
        CEmutex_lock(car_mutex); //bloquea el mutex , asi al crear el hilo no 
        //se va a ejecutar de una
        car->mutex =car_mutex;
        car->id = id++;
        car->type = 1;
        car->direction = 0; // 0: izquierda a derecha, 1: derecha a izquierda
        car->carTexture = carSport;
        //genera el hilo
        CEthread_t thread;
        int tid = CEthread_create(car_function, car);
        thread.tid = tid;
        void *retval;
        CEthread_join(thread, &retval);
        //end generar hilo, esta dormido ya que inicio con el mutex tomado
        //se necesita un algoritmo descolador que le haga pop a la cola, y haga
        //car ->unlock mutex.
        encolar_con_algoritmo(car,0); //lo encola

    }
    for (int j = 0; j < normales; j++) {
        //al carro hay que anadirle la textura sdl
        Car* car = malloc(sizeof(Car));
        car_mutex= malloc(sizeof(CEmutex_t)); //crea el mutex
        CEmutex_init(car_mutex); //inicia el mutex
        CEmutex_lock(car_mutex); //bloquea el mutex , asi al crear el hilo no 
        //se va a ejecutar de una
        car->mutex =car_mutex;
        car->id = id++;
        car->type = 0;
        car->direction = 1; // 0: izquierda a derecha, 1: derecha a izquierda
        car->carTexture = carNormal;
        //genera el hilo
        CEthread_t thread;
        int tid = CEthread_create(car_function, car);
        thread.tid = tid;
        void *retval;
        CEthread_join(thread, &retval);
        //end generar hilo, esta dormido ya que inicio con el mutex tomado
        //se necesita un algoritmo descolador que le haga pop a la cola, y haga
        //car ->unlock mutex.
        encolar_con_algoritmo(car,1); //lo encola

    }
    for (int j = 0; j < normales; j++) {
        //al carro hay que anadirle la textura sdl
        Car* car = malloc(sizeof(Car));
        car_mutex= malloc(sizeof(CEmutex_t)); //crea el mutex
        CEmutex_init(car_mutex); //inicia el mutex
        CEmutex_lock(car_mutex); //bloquea el mutex , asi al crear el hilo no 
        //se va a ejecutar de una
        car->mutex =car_mutex;
        car->id = id++;
        car->type = 0;
        car->direction = 0; // 0: izquierda a derecha, 1: derecha a izquierda
        car->carTexture = carNormal;
        //genera el hilo
        CEthread_t thread;
        int tid = CEthread_create(car_function, car);
        thread.tid = tid;
        void *retval;
        CEthread_join(thread, &retval);
        //end generar hilo, esta dormido ya que inicio con el mutex tomado
        //se necesita un algoritmo descolador que le haga pop a la cola, y haga
        //car ->unlock mutex.
        encolar_con_algoritmo(car,0); //lo encola

    }
    for (int j = 0; j < ambulancias; j++) {
        //al carro hay que anadirle la textura sdl
        Car* car = malloc(sizeof(Car));
        car_mutex= malloc(sizeof(CEmutex_t)); //crea el mutex
        CEmutex_init(car_mutex); //inicia el mutex
        CEmutex_lock(car_mutex); //bloquea el mutex , asi al crear el hilo no 
        //se va a ejecutar de una
        car->mutex =car_mutex;
        car->id = id++;
        car->type = 2;
        car->direction = 1; // 0: izquierda a derecha, 1: derecha a izquierda
        car->carTexture = carAmbulance;
        //genera el hilo
        CEthread_t thread;
        int tid = CEthread_create(car_function, car);
        thread.tid = tid;
        void *retval;
        CEthread_join(thread, &retval);
        //end generar hilo, esta dormido ya que inicio con el mutex tomado
        //se necesita un algoritmo descolador que le haga pop a la cola, y haga
        //car ->unlock mutex.
        encolar_con_algoritmo(car,1); //lo encola

    }
    for (int j = 0; j < ambulancias j++) {
        //al carro hay que anadirle la textura sdl
        Car* car = malloc(sizeof(Car));
        car_mutex= malloc(sizeof(CEmutex_t)); //crea el mutex
        CEmutex_init(car_mutex); //inicia el mutex
        CEmutex_lock(car_mutex); //bloquea el mutex , asi al crear el hilo no 
        //se va a ejecutar de una
        car->mutex =car_mutex;
        car->id = id++;
        car->type = 2;
        car->direction = 0; // 0: izquierda a derecha, 1: derecha a izquierda
        car->carTexture = carAmbulance;
        //genera el hilo
        CEthread_t thread;
        int tid = CEthread_create(car_function, car);
        thread.tid = tid;
        void *retval;
        CEthread_join(thread, &retval);
        //end generar hilo, esta dormido ya que inicio con el mutex tomado
        //se necesita un algoritmo descolador que le haga pop a la cola, y haga
        //car ->unlock mutex.
        encolar_con_algoritmo(car,0); //lo encola

    }
    //normales son 0, ambulancias son 2
    //RECUERDE, DEBE DE HACERLE FREE(CAR), cuando el carro complete segun el algoritmo!!!
    //aca tambien debo de crear los hilos, con su mutex muerto.
}
void initQueue(int tipoCalendarizador){
    seleccionar_algoritmo(tipoCalendarizador); //inicializa las colas.
    //el 3 es el calendarizado de mariana
}







/*
//#define NUM_CARROS 

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
    seleccionar_algoritmo(opcionCalendarizador);
    
    int contadorDeportivos = cantidadDeportivos;
    int contadorNormales = cantidadNormales;
    int contadorEmergencia = cantidadEmergencia;

    // crear y encolar los carros
    for (int i = 0; i < cantidadCarros; i++) {
        Car *car = malloc(sizeof(Car));
        car->id = i + 1;
        if (contadorDeportivos > 0) {
            car->type = 1; // sport
            contadorDeportivos--;
        } else if (contadorNormales > 0) {
            car->type = 0; // normal
            contadorNormales--;
        } else if (contadorEmergencia > 0) {
            car->type = 2; // emergency
            contadorEmergencia--;
        }
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
