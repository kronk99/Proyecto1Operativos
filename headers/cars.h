#ifndef CAR_H
#define CAR_H

#include "CEthreads.h"

struct Car {
    int id;
    int position;
    int side;
    int speed;
    int priority;
    int burstTime;
    int type;
    CEthread_t thread;
};

struct CarList {
    struct Car* cars;
    int count;
    int capacity;
};

void initCarList(struct CarList* list, int capacity);
void addCar(struct CarList* list, struct Car car);
void freeCarList(struct CarList* list);
struct Car createCar(int id, int side, int speed, int priority, int burstTime, int type);

#endif
