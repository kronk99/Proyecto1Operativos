#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "../headers/cars.h"

pthread_mutex_t road_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t road_available = PTHREAD_COND_INITIALIZER;

int current_direction = -1;  // -1: libre, 0: izquierda, 1: derecha
int cars_on_road = 0;

// ======================= EQUITY =========================
void* equity_car_thread(void* arg) {
    struct Car* c = (struct Car*)arg;

    pthread_mutex_lock(&road_mutex);

    while (current_direction != -1 && current_direction != c->side) {
        pthread_cond_wait(&road_available, &road_mutex);
    }

    current_direction = c->side;
    cars_on_road++;
    pthread_mutex_unlock(&road_mutex);

    printf("Carro %d cruzando desde %s...\n", c->id, c->side == 0 ? "IZQUIERDA" : "DERECHA");
    sleep(c->burstTime);
    printf("Carro %d ha cruzado.\n", c->id);

    pthread_mutex_lock(&road_mutex);
    cars_on_road--;
    if (cars_on_road == 0) {
        current_direction = -1;
        pthread_cond_broadcast(&road_available);
    }
    pthread_mutex_unlock(&road_mutex);

    free(c);
    return NULL;
}

void equity(struct CarList* left, struct CarList* right, int w) {
    int left_index = 0;
    int right_index = 0;

    while (left_index < left->count || right_index < right->count) {
        // Turno izquierda
        int i;
        for (i = 0; i < w && left_index < left->count; i++) {
            struct Car* c = malloc(sizeof(struct Car));
            *c = left->cars[left_index++];
            pthread_t tid;
            pthread_create(&tid, NULL, equity_car_thread, (void*)c);
            pthread_detach(tid);
        }
        sleep(1); // Pequeña pausa para permitir el cruce

        // Turno derecha
        for (i = 0; i < w && right_index < right->count; i++) {
            struct  Car* c = malloc(sizeof(struct Car));
            *c = right->cars[right_index++];
            pthread_t tid;
            pthread_create(&tid, NULL, equity_car_thread, (void*)c);
            pthread_detach(tid);
        }
        sleep(1);
    }

    // Esperar a que todos los carros terminen
    while (cars_on_road > 0) {
        sleep(1);
    }
}

// ======================= LETRERO =========================
int sing_direction = 0; // 0: izquierda, 1: derecha

void* sing_car_thread(void* arg) {
    struct Car* c = (struct Car*)arg;

    pthread_mutex_lock(&road_mutex);
    while (sing_direction != c->side || cars_on_road > 0) {
        pthread_cond_wait(&road_available, &road_mutex);
    }

    cars_on_road++;
    pthread_mutex_unlock(&road_mutex);

    printf("Carro %d cruzando desde %s...\n", c->id, c->side == 0 ? "IZQUIERDA" : "DERECHA");
    sleep(c->burstTime);
    printf("Carro %d ha cruzado.\n", c->id);

    pthread_mutex_lock(&road_mutex);
    cars_on_road--;
    if (cars_on_road == 0) {
        pthread_cond_broadcast(&road_available);
    }
    pthread_mutex_unlock(&road_mutex);

    free(c);
    return NULL;
}

void sing(struct CarList* left, struct CarList* right, int interval) {
    int left_index = 0;
    int right_index = 0;

    while (left_index < left->count || right_index < right->count) {
        if (sing_direction == 0 && left_index < left->count) {
            struct Car* c = malloc(sizeof(struct Car));
            *c = left->cars[left_index++];
            pthread_t tid;
            pthread_create(&tid, NULL, sing_car_thread, (void*)c);
            pthread_detach(tid);
        }

        if (sing_direction == 1 && right_index < right->count) {
            struct Car* c = malloc(sizeof(struct Car));
            *c = right->cars[right_index++];
            pthread_t tid;
            pthread_create(&tid, NULL, sing_car_thread, (void*)c);
            pthread_detach(tid);
        }

        sleep(1); // Intentar cruzar con cada iteración
        interval--;
        if (interval <= 0) {
            pthread_mutex_lock(&road_mutex);
            sing_direction = !sing_direction;
            printf("==> LETRERO CAMBIÓ A: %s\n", sing_direction == 0 ? "IZQUIERDA" : "DERECHA");
            pthread_cond_broadcast(&road_available);
            pthread_mutex_unlock(&road_mutex);
            interval = 5; // Reiniciar intervalo de ejemplo
        }
    }

    while (cars_on_road > 0) {
        sleep(1);
    }
}

void* car_thread_fifo(void* arg) {
    struct Car* car = (struct Car*)arg;

    // Espera su turno para entrar a la carretera
    pthread_mutex_lock(&road_mutex);

    printf("Carro %d entrando a la carretera desde el lado %s\n", car->id,
           car->side == 0 ? "Izquierdo" : "Derecho");

    // Simula el tiempo que tarda en cruzar
    sleep(car->burstTime);

    printf("Carro %d salió de la carretera\n", car->id);

    pthread_mutex_unlock(&road_mutex);
    return NULL;
}


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