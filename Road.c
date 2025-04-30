#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "Cars/cars.h"

#define NUM_CARS_LEFT 3
#define NUM_CARS_RIGHT 3

pthread_mutex_t road_mutex = PTHREAD_MUTEX_INITIALIZER;

void* car_thread(void* arg) {
    struct car* c = (struct car*)arg;

    const char* side_str = c->side == 0 ? "LEFT" : "RIGHT";
    printf("Car %d from %s is waiting to enter the road\n", c->id, side_str);

    pthread_mutex_lock(&road_mutex);
    printf("Car %d from %s is ENTERING the road\n", c->id, side_str);

    // Simula cruce de la carretera
    sleep(c->burstTime);

    printf("Car %d from %s has EXITED the road\n", c->id, side_str);
    pthread_mutex_unlock(&road_mutex);

    free(c);
    pthread_exit(NULL);
}

int main() {
    struct car* cars[NUM_CARS_LEFT + NUM_CARS_RIGHT];
    int total = 0;

    // Crear carros desde la izquierda
    for (int i = 0; i < NUM_CARS_LEFT; i++) {
        struct car* c = malloc(sizeof(struct car));
        c->id = total + 1;
        c->side = 0;
        c->burstTime = 1 + rand() % 3; // 1 a 3 segundos
        c->speed = 50 + rand() % 50;
        c->priority = rand() % 10;
        c->position = 0;
        c->type = 1;
        pthread_create(&c->thread, NULL, car_thread, c);
        cars[total++] = c;
        usleep(200000);
    }

    // Crear carros desde la derecha
    for (int i = 0; i < NUM_CARS_RIGHT; i++) {
        struct car* c = malloc(sizeof(struct car));
        c->id = total + 1;
        c->side = 1;
        c->burstTime = 1 + rand() % 3;
        c->speed = 50 + rand() % 50;
        c->priority = rand() % 10;
        c->position = 100;
        c->type = 2;
        pthread_create(&c->thread, NULL, car_thread, c);
        cars[total++] = c;
        usleep(200000);
    }

    // Esperar a que todos los hilos terminen
    for (int i = 0; i < total; i++) {
        pthread_join(cars[i]->thread, NULL);
    }

    pthread_mutex_destroy(&road_mutex);
    return 0;
}
