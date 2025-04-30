#include <pthread.h>

struct car {
    int id;
    int position;
    int side;       // 0 = izquierda, 1 = derecha
    int speed;
    int priority;
    int burstTime;  // Tiempo para cruzar
    int type;
    pthread_t thread;
};

struct car* create_car(int id, int side);

