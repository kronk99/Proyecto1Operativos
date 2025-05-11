#include <stdio.h>
#include <stdlib.h>
#include "../headers/cars.h"
#include "../headers/CEthreads.h"

void initCarList(struct CarList* list, int capacity) {
    list->cars = (struct Car*)malloc(sizeof(struct Car) * capacity);
    list->count = 0;
    list->capacity = capacity;
}

void addCar(struct CarList* list, struct Car car) {
    if (list->count < list->capacity) {
        list->cars[list->count++] = car;
    } else {
        // Puedes redimensionar si lo deseas:
        list->capacity *= 2;
        list->cars = realloc(list->cars, sizeof(struct Car) * list->capacity);
        list->cars[list->count++] = car;
    }
}

void freeCarList(struct CarList* list) {
    if (list->cars != NULL) {
        free(list->cars);
        list->cars = NULL;
    }
    list->count = 0;
    list->capacity = 0;
}

struct Car createCar(int id, int side, int speed, int priority, int burstTime, int type) {
    struct Car newCar;
    newCar.id = id;
    newCar.position = 0;
    newCar.side = side;
    newCar.speed = speed;
    newCar.priority = priority;
    newCar.burstTime = burstTime;
    newCar.type = type;
    newCar.thread.tid = -1;
    return newCar;
}
