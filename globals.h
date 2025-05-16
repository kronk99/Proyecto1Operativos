// globals.h
#ifndef GLOBALS_H
#define GLOBALS_H

#include "Calendarizador/ReadyQueue.h"  // Asegurate que este incluye la definición de ReadyQueue
#include <SDL2/SDL.h>

extern ReadyQueue global_queue;
extern ReadyQueue global_queueLeft;
extern SDL_Texture* sentidoIzquierda;
extern SDL_Texture* sentidoDerecha;

#endif
