#include "interfaz.h"
#include "Testing.h"  
#include "Flow/flow.h"
#include "Calendarizador/Scheduler.h"
#include "Calendarizador/ReadyQueue.h"
#include "Lectura/read.h"
#include <stdio.h>
#include <string.h>
#define STACK_SIZE (1024 * 1024) 

ReadyQueue global_queue;
ReadyQueue global_queueLeft;

int main(int argc, char* argv[]) {

    TrafficConfig config;

    readConfiguration("configuracion.txt", &config);

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

    seleccionar_algoritmo(config.tipo_calendarizador, &global_queue, &global_queueLeft);
    createCars(config.carros_deportivos, config.carros_emergencia, config.carros_normales, carSport, carEmergency, carNormal, &global_queue, &global_queueLeft);

    // Dibujar escena inicial
    // Va a pintar los carros que estan esperando para pasar.
    dibujarEscenario(renderer, &global_queue, &global_queueLeft, config.largo_calle);

        //crea el relog
    //llama al letrero
    //vamos a ver si funciona
    //letrero(&global_queue, &global_queueLeft, &tiempo);
    //

    //Tipo de flujo seleccionado es equidad
    if(config.tipo_flujo == 0){
        printf("Tipo de flujo seleccionado es equidad\n");
        equity(config.w, &global_queue, &global_queueLeft);
    }
    //Tipo de flujo seleccionado es letrero
    else if (config.tipo_flujo == 1)
    {
        Timer tiempo;
        atomic_init(&tiempo.dirLetrero, 0);
        atomic_init(&tiempo.timeCount, config.tiempo_letrero);
        tiempo.time_Value = config.tiempo_letrero;

        CEthreads_t hilo;
        hilo.start_routine = contador;
        hilo.arg = &tiempo;
        hilo.stack_size = STACK_SIZE;
        hilo.is_finished = 0;
        printf("Tipo de flujo seleccionado es letrero\n");
        letrero(&global_queue, &global_queueLeft, &tiempo);
        

    }
    //Tipo de flujo seleccionado FIFO
    else{
        printf("Tipo de flujo seleccionado FIFO\n");

    }


    cerrarInterfaz(window, renderer, carSport, carNormal, carEmergency);
    return 0;
}


