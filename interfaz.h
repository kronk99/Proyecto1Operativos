#ifndef INTERFAZ_H
#define INTERFAZ_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Car.h"  

// Dimensiones ventana
#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 600

// Funciones
int iniciarInterfaz(SDL_Window** window, SDL_Renderer** renderer);
SDL_Texture* cargarCarro(SDL_Renderer* renderer, const char* path);
void dibujarEscenario(SDL_Renderer* renderer, SDL_Texture* carSport, SDL_Texture* carNormal, SDL_Texture* carEmergency, Car** carros, int cantidadCarros, int largoCalle);
void cerrarInterfaz(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* carSport, SDL_Texture* carNormal, SDL_Texture* carEmergency);

#endif
