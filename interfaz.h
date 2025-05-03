#ifndef INTERFAZ_H
#define INTERFAZ_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Dimensiones ventana
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// Funciones
int iniciarInterfaz(SDL_Window** window, SDL_Renderer** renderer);
SDL_Texture* cargarCarro(SDL_Renderer* renderer, const char* path);
void dibujarEscenario(SDL_Renderer* renderer, SDL_Texture* carTexture);
void cerrarInterfaz(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* carTexture);

#endif
