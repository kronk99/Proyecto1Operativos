#include "interfaz.h"
#include "Testing.h"  
#include "Flow/flow.h"
#include "Calendarizador/Scheduler.h"
#include "Calendarizador/ReadyQueue.h"
#include <stdio.h>
#include <string.h>
#define STACK_SIZE (1024 * 1024) 
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
    createCars(2, 2, 0, carSport, carEmergency, carNormal, &global_queue, &global_queueLeft);

    // Suponemos que tenés un array de carros, por ahora NULL para evitar error
    Car* carros[10] = {0};  // o el arreglo que uses
    int cantidadCarros = 0; // o el número real de carros creados
    int largoCalle = 600;

    // Dibujar escena inicial
    dibujarEscenario(renderer, carSport, carNormal, carEmergency, carros, cantidadCarros, largoCalle);


    equity(2, &global_queue, &global_queueLeft, carSport, carNormal, carEmergency);


    cerrarInterfaz(window, renderer, carSport, carNormal, carEmergency);
    return 0;
}


