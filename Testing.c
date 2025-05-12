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
    //es decir la funcion que va a descalendarizar los carros
    atomic_store(&(car->hasArrived), 0); //RECUERDE INICIALIZAR ANTES DE SETEARLA
    while(!atomic_load(&(car->hasArrived))){
        if(car->direction==1){ //si la direccion es de derecha a izquierda
            atomic_fetch_sub(&(car->positionX), 1); //le resta 1 a la posicion actual
             printf("carro cruzando a la izquierda");
            if(atomic_load(&(car->positionX)) < 20){ //si es menor a 20
                atomic_store(&(car->hasArrived), 1);
               
            }
            sleep(1); //duerme un poco
            //llama a la funcion pintar carro
        }
        else{ //si la direccion es de izquierda a derecha
            atomic_fetch_add(&(car->positionX), 1);
             printf("carro cruzanndndo a la derecha");
            if(atomic_load(&(car->positionX)) > 400){ //si es menor al maximo valor de la calle.
                //este valor deberia ser accesible para todos, cambiarlo luego para la prueba
                atomic_store(&(car->hasArrived), 1);
            }
            sleep(1); //debe dormir una cantidad 
            //llama al paintAll, si no , que el paintall tambien sea un hilo y accedo al 
            //car position x al pintarlo, solo que si se llama paintall desde otro lado
            //debe de haber concurrencia al pintar entre hilos, para que le de tiempo de actualizar
            //las posiciones 
        }
        
    }
    printf("Carro #%d ha cruzado (mutex liberado)\n", car->id);
    CEmutex_unlock(car->mutex);
    //CEmutex_unlock(mutex);
    //pthread_exit(NULL);  // termina el hilo correctamente
  // mata el hilo (nunca retorna)
    //deberia de destuir el hilo
    //aca hace falta un metodo de pintado global ()paintall
    //free(car); el free lo hace en el manejador de direccion
    return NULL;
}


int flujo_carritos(void* arg) {
    //Ya para este punto se tuvo que haber pasado por el calendarizador, entonces, agarro los carros en ese orden
    //Necesito agarrar el carro y entonces aplicarle CEmutex_unlock para indicar que ese carro(hilo) tiene permiso para ejecutarse
    int currentSide = 1; // Comenzamos con los carros de la izquierda
    int tipo_flujo=1; //cambiado para que funque
    int W =2;
    FlujoArgs *args = (FlujoArgs *)arg;
    ReadyQueue *global_queue = args->queueRight;
    ReadyQueue *global_queueLeft = args->queueLeft;
    switch (tipo_flujo) {
        case 1:
            /*
            consiste en establecer un parámetro W (indicado por el usuario) que indica cuántos carros deben de 
            pasar de cada lado. Es decir, se inicia permitiendo el paso de W carros de izquierda a derecha, y luego W carros de
            derecha a izquierda. En caso de que en alguno de los lados no haya carros, se debe garantizar el flujo 
            de vehículos desde el lado donde sí los haya.
            */
           int count = 0;
            //Logica
            // Agarro el primer hilo de la lista calendarizada. 
            // Llamo a CEmutex_unlock para establecer que esta listo para ejecutarse el hilo
            // Lo pinto en las coordenadas establecidas ya sean del lado derecho o del lado izquierdo
            // Lo pinto y lo voy moviendo, la verificacion es con las coordenadas de limite, estas hay que agregarlas de alguna manera tal vez
            // estableciendolas desde este archivo testing.
            // Una vez que termina debo elimnar la imagen, destruir el mutex, el hilo y liberar el espacio
            //Este while es para estar verificando que la lista no este vacia
            bool value = true;
            printf("antes del while \n");
            count = W; // reseteo el contador
            while(value){ //esto es un while true;
                printf("entre al while\n");
                if(currentSide ==1){ //si esla derecha
                    if(!is_empty(&global_queue)){ //si la cola no esta vacia
                        //hago dequeue
                        sleep(10);
                        printf("hago dequeue\n");
                        Car *currentcar = dequeue(&global_queue);
                        CEmutex_unlock(currentcar->mutex);
                        //atomic_flag_clear_explicit(&(currentcar->mutex->lock), memory_order_release);
                        //que el carro avance., en estos momentos el carro deberia de estar
                        //aca deberia de ponerlo en 0
                        //printf("el valor es %d \n",currentcar->mutex->lock);
                        //avanzando en la carretera.
                        printf("Se desbloqueo el mutex\n");
                        while(!atomic_load(&(currentcar->hasArrived))){ //se pone a esperar a que el carro llegue al otro lado
                            //espera a que le indiquen que el carro ha llegado
                            //printf("Carro cruzando: %d\n", currentcar->id);
                            //falta destruir carro y mutex
                            //printf("Sexoooooo \n");
                            sleep(5);
                            //printf("el valor DEL MUTEX EN DEQUEUE ES: %d",currentcar->mutex->lock);
                        }
                        printf("CARRO HA TERMINADO DE CRUZAR Y SE VA A ELIMINAR \n" );

//VOY A HACER UNA PRUEBA : CAMBIANDO EL CETHREADS A PTHREADS
//

                        //cuando sale de esperar el carro, le resta al count
                        count -=1; //le quita1 al contador
                        //checkea si el count llego a 0 para el cambio
                        if(count ==0){
                            currentSide = 0; //cambia de lado a cola izquierda
                            count = W; // reseteo el contador
                        }
                    }
                    else{ //si esta vacia, cambio de lado
                        printf("cambie de lado \n"); 
                        currentSide = 0; //cambia de lado a cola izquierda
                        count = W; // reseteo el contador
                    }
                }
                else{//si es izquierda
                    if(!is_empty(&global_queueLeft)){ //si la cola no esta vacia
                        //hago dequeue
                        Car *currentcar = dequeue(&global_queueLeft);
                        CEmutex_unlock(currentcar->mutex); //desbloquea el mutex para 
                        //que el carro avance., en estos momentos el carro deberia de estar
                        //avanzando en la carretera.
                        while(!atomic_load(&(currentcar->hasArrived))){ //se pone a esperar a que el carro llegue al otro lado
                            //espera a que le indiquen que el carro ha llegado
                            

                        }
                        printf("Carro termino de cruzar: %d\n", currentcar->id);
                        //cuando sale de esperar el carro, le resta al count
                        count -=1; //le quita1 al contador
                        //checkea si el count llego a 0 para el cambio
                        //Destruyo el carro y su mutex (Dado que no se debe de reencolar)

                        if(count ==0){
                            currentSide = 1; //cambia de lado a cola derecha
                            count = W; // reseteo el contador
                        }
                    }
                    else{ //si esta vacia, cambio de lado 
                        currentSide = 1; //cambia de lado a cola izquierda
                        count = W; // reseteo el contador
                }
            }
            
            // Aquí va la implementación de Equidad
            break;

        case 2:
            // FIFO: lógica para dejar pasar los carros en el orden en que llegaron
            printf("Aplicando flujo de tipo FIFO...\n");
            // Aquí va la implementación de FIFO
            break;

        case 3:
            // Letrero: Puede presentar problemas debido a que, segun el tiempo establecido para que pasen los carros, puede
            // que alguno quede en medio camino, entonces hay que guardar esas coordenadas, volver a guardar el carro en la cola 
            // y para la proxima entonces el carro comeinza desde las coordenadas que quedó anteriormente.
            printf("Flujo de tipo Letrero aún no implementado.\n");
            break;

        default:
            printf("Tipo de flujo no válido: %d\n", tipo_flujo);
            break;
        }
    }
}
/*
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

*/

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
        car->id = id++;
        car->type = 1;
        car->direction = 1; // derecha a izquierda
        car->carTexture = carSport;

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
