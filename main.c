#include "interfaz.h"
#include "Testing.h"  
#include "Flow/flow.h"
#include "Calendarizador/Scheduler.h"
#include "Calendarizador/ReadyQueue.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
    ReadyQueue global_queue;
    ReadyQueue global_queueLeft;

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    if (!iniciarInterfaz(&window, &renderer)) return 1;
    SDL_Texture* carSport = cargarCarro(renderer, "images/sport.png");
    SDL_Texture* carNormal = cargarCarro(renderer, "images/normal.png");
    SDL_Texture* carEmergency = cargarCarro(renderer, "images/emergency.png");
    if (!carSport || !carNormal || !carEmergency) {
        cerrarInterfaz(window, renderer, carSport, carNormal, carEmergency);
        return 1;
    }

    seleccionar_algoritmo(1, &global_queue, &global_queueLeft);
    createCars(2, 2, 2, carSport, carEmergency, carNormal, &global_queue, &global_queueLeft);

    printf("hola\n");

    equity(2, &global_queue, &global_queueLeft);

    int quit = 0;
    SDL_Event e;

    return 0;
}

