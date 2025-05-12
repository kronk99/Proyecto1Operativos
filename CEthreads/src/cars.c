#include <stdio.h>
#include <stdlib.h>
#include "../CEthreads/include/Car.h"


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
// Función para crear un nuevo carro
Car* createCar(int id, int position, int speed, int priority, int burstTime,
               int type, int deadline, int direction, SDL_Texture* texture) {
    Car* newCar = malloc(sizeof(Car));
    if (newCar == NULL) {
        perror("No se pudo asignar memoria para el carro");
        return NULL;
    }

    newCar->id = id;
    //newCar->position = position;
    newCar->speed = speed;
    newCar->priority = priority;
    newCar->burstTime = burstTime;
    newCar->type = type;
    newCar->deadline = deadline;
    newCar->direction = direction;
    newCar->carTexture = texture;
    atomic_init(&(newCar->hasArrived), 0); //setea la bandera de llegado en 0

    // Crear e inicializar mutex propio del carro

    return newCar;
}

