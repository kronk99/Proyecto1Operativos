#include <stdlib.h>
#include <time.h>
#include <cars.h>

struct car* create_car(int id, int side) {
    struct car* c = malloc(sizeof(struct car));
    if (!c) {
        perror("Failed to allocate car");
        exit(EXIT_FAILURE);
    }

    c->id = id;
    c->side = side;
    c->position = (side == 0) ? 0 : 100;
    c->speed = 50 + rand() % 50;      // Velocidad entre 50 y 99
    c->priority = rand() % 10;        // Prioridad entre 0 y 9
    c->burstTime = 1 + rand() % 3;    // Entre 1 y 3 segundos para cruzar
    c->type = side + 1;               // Tipo 1 o 2 dependiendo del lado
    return c;
}
