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
    int total = left->count + right->count;

    // Crear lista combinada de carros
    struct Car** allCars = malloc(sizeof(struct Car*) * total);
    int index = 0;

    // Copiar los carros de la izquierda
    for (int i = 0; i < left->count; i++) {
        allCars[index++] = &left->cars[i];
    }

    // Copiar los carros de la derecha
    for (int i = 0; i < right->count; i++) {
        allCars[index++] = &right->cars[i];
    }

    // Ordenar carros por su ID (FIFO)
    for (int i = 0; i < total - 1; i++) {
        for (int j = i + 1; j < total; j++) {
            if (allCars[i]->id > allCars[j]->id) {
                struct Car* temp = allCars[i];
                allCars[i] = allCars[j];
                allCars[j] = temp;
            }
        }
    }

    // Crear hilos para cada carro en orden FIFO
    for (int i = 0; i < total; i++) {
        pthread_create(&allCars[i]->thread, NULL, car_thread_fifo, allCars[i]);
    }

    // Esperar que todos los hilos terminen
    for (int i = 0; i < total; i++) {
        pthread_join(allCars[i]->thread, NULL);
    }

    // Liberar memoria de la lista combinada
    free(allCars);
}