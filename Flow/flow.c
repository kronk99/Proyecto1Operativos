#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../Threads/Car.h"
#include "../Threads/CEthread.h"
#include "../Calendarizador/Scheduler.h"
#include "../Calendarizador/ReadyQueue.h"
#include "../interfaz.h"
#include "flow.h"
#include "../globals.h"


int current_direction = -1;  // -1: libre, 0: izquierda, 1: derecha


void equity(int w, ReadyQueue* queueRight, ReadyQueue* queueLeft) {
    while (!is_empty(queueRight) || !is_empty(queueLeft)) {
        // Turno izquierda a derecha
        for (int i = 0; i < w && !is_empty(queueLeft); i++) {
            Car* car = siguiente_carro(queueLeft);
            printf("Equity desbloqueando carro #%d (izquierda a derecha)\n", car->id);
            CEmutex_unlock(car->mutex);

            //draw_car(car->type, car->direction, carSport, carNormal, carEmergency);

            while(atomic_load(&car->hasArrived) == 0){
                sleep(1); // Dejar que este carro cruce solo antes de dar paso al siguiente
            }
            //CEmutex_destroy(car->mutex);
            //free(car);
        }

        // Turno derecha a izquierda
        for (int i = 0; i < w && !is_empty(queueRight); i++) {
            Car* car = siguiente_carro(queueRight);
            printf("Equity desbloqueando carro #%d (derecha a izquierda)\n", car->id);
            CEmutex_unlock(car->mutex);
            //draw_car(car->type, car->direction, carSport, carNormal, carEmergency);


            while(atomic_load(&car->hasArrived) == 0){
                sleep(1); // Dejar que este carro cruce solo antes de dar paso al siguiente
            }
            //CEmutex_destroy(car->mutex);
            //free(car);
        }
    }

    fprintf(stderr, "Todos los carros han cruzado.\n");
}

void draw_car(int tipo, int direccion, SDL_Texture* imagen_carro) {
    SDL_Texture* textura = NULL;

    textura = imagen_carro;

    int y = WINDOW_HEIGHT / 2 - 15; // centro vertical
    int w = 70, h = 40;             // tamaño del carro
    int startX, endX, step;

    if (direccion == 0) { // izquierda a derecha
        startX = 0;
        endX = WINDOW_WIDTH;
        if(tipo == 0){
            step = 2;
        }
        else if (tipo == 1)
        {
            step = 3;
        }
        else{
            step = 5;
        }
        
    } else {              // derecha a izquierda
        startX = WINDOW_WIDTH;
        endX = 0 - w;
        if(tipo == 0){
            step = -2;
        }
        else if (tipo == 1)
        {
            step = -3;
        }
        else{
            step = -5;
        }
    }

    for (int x = startX; (direccion == 0 ? x < endX : x > endX); x += step) {
        SDL_SetRenderDrawColor(globalRenderer, 0, 0, 0, 255);
        SDL_RenderClear(globalRenderer);
        dibujarEscenario(globalRenderer, &global_queue, &global_queueLeft,  600);

        SDL_Rect carRect = {x, y, w, h};
        if (direccion == 0)
            SDL_RenderCopyEx(globalRenderer, textura, NULL, &carRect, 0, NULL, SDL_FLIP_HORIZONTAL);
        else
            SDL_RenderCopy(globalRenderer, textura, NULL, &carRect);

        SDL_RenderPresent(globalRenderer);
        SDL_Delay(10); // control de velocidad
    }
}




// ======================= LETRERO =========================

int contador(void *arg){
    printf("--------------------ENTRA EN CONTADOR--------------------\n");
    Timer *tiempo = (Timer *)arg;
    int value=1;
    while (value){
        atomic_fetch_sub(&(tiempo->timeCount), 1);//le resta 1 
            //y hace sleep
        sleep(1); //mete un sleep para simular el conteo
        if(tiempo->timeCount ==0){
            printf("----------------Cambio de direccion de letrero----------------\n ");
            atomic_fetch_xor(&(tiempo->dirLetrero), 1); //si es 1 lo pone en 0 y viceversa
            dibujarFlechaDireccion(globalRenderer, sentidoIzquierda, sentidoDerecha, tiempo->dirLetrero);
            // timeCount = 10
            atomic_store(&(tiempo->timeCount), tiempo->time_Value);  // resetea el contador
        }
    }
}
    
void letrero(ReadyQueue* queueRight, ReadyQueue* queueLeft, Timer *tiempo){
    //int sing_direction = 0; // 0: izquierda, 1: derecha
    while (!is_empty(queueRight) || !is_empty(queueLeft)) {// Turno izquierda a derecha
        if(atomic_load(&(tiempo->dirLetrero))==0 && !is_empty(queueLeft)) {
            Car* car = siguiente_carro(queueLeft);
            printf("Letrero desbloqueando carro #%d (izquierda a derecha)\n", car->id);
            CEmutex_unlock(car->mutex);

            //draw_car(car->type, car->direction, carSport, carNormal, carEmergency);

            while(atomic_load(&car->hasArrived) == 0){
                sleep(1); // Dejar que este carro cruce solo antes de dar paso al siguiente
            }
            //CEmutex_destroy(car->mutex);
            //free(car);
        }
        else if(atomic_load(&(tiempo->dirLetrero))==1 && !is_empty(queueRight)){
            Car* car = siguiente_carro(queueRight);
            printf("Letrero desbloqueando carro #%d (izquierda a derecha)\n", car->id);
            CEmutex_unlock(car->mutex);

            //draw_car(car->type, car->direction, carSport, carNormal, carEmergency);

            while(atomic_load(&car->hasArrived) == 0){
                sleep(1); // Dejar que este carro cruce solo antes de dar paso al siguiente
            }
            //CEmutex_destroy(car->mutex);
            //free(car);
        }
        //hago cambio al timer:
    }
    fprintf(stderr, "Todos los carros han cruzado.\n");
}
void fifo(int w, ReadyQueue* queueRight, ReadyQueue* queueLeft) {
    while (!is_empty(queueRight) || !is_empty(queueLeft)) {
        Car* leftCar = peek_car(queueLeft);   // Ver primer carro en la cola izquierda
        Car* rightCar = peek_car(queueRight); // Ver primer carro en la cola derecha

        Car* nextCar = NULL;
        int fromLeft = 0;

        if (leftCar && rightCar) {
            if (leftCar->id < rightCar->id) {
                nextCar = siguiente_carro(queueLeft); // Remueve de la cola
                fromLeft = 1;
            } else {
                nextCar = siguiente_carro(queueRight); // Remueve de la cola
                fromLeft = 0;
            }
        } else if (leftCar) {
            nextCar = siguiente_carro(queueLeft);
            fromLeft = 1;
        } else if (rightCar) {
            nextCar = siguiente_carro(queueRight);
            fromLeft = 0;
        }

        if (nextCar) {
            printf("Equity desbloqueando carro #%d (%s)\n", nextCar->id, fromLeft ? "izquierda a derecha" : "derecha a izquierda");
            CEmutex_unlock(nextCar->mutex);

            while (atomic_load(&nextCar->hasArrived) == 0) {
                sleep(1); // Esperar a que cruce antes de desbloquear el siguiente
            }
        }
    }

    fprintf(stderr, "Todos los carros han cruzado.\n");
}
/*
void fifo(struct CarList* left, struct CarList* right) {
while (left->count > 0 || right->count > 0) {
    int left_has = left->count > 0;
    int right_has = right->count > 0;

    struct Car* selected_car = NULL;
    int from_left = 0;

    if (left_has && right_has) {
        struct Car* left_car = &left->cars[0];
        struct Car* right_car = &right->cars[0];

        if (left_car->id < right_car->id) {
            selected_car = left_car;
            from_left = 1;
        } else {
            selected_car = right_car;
            from_left = 0;
        }
    } else if (left_has) {
        selected_car = &left->cars[0];
        from_left = 1;
    } else if (right_has) {
        selected_car = &right->cars[0];
        from_left = 0;
    }

    if (selected_car) {
        pthread_t tid;
        // Creamos una copia local del carro porque lo vamos a eliminar de la lista
        struct Car car_copy = *selected_car;

        pthread_create(&tid, NULL, car_thread_fifo, (void*)&car_copy);
        pthread_join(tid, NULL);  // Esperamos a que cruce

        // Eliminar de la lista original
        if (from_left) {
            for (int i = 0; i < left->count - 1; i++) {
                left->cars[i] = left->cars[i + 1];
            }
            left->count--;
        } else {
            for (int i = 0; i < right->count - 1; i++) {
                right->cars[i] = right->cars[i + 1];
            }
            right->count--;
        }
    }
}
}
*/
