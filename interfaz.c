#include "interfaz.h"
#include <stdio.h>

int iniciarInterfaz(SDL_Window** window, SDL_Renderer** renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error inicializando SDL: %s\n", SDL_GetError());
        return 0;
    }

    *window = SDL_CreateWindow("Interfaz Calle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!*window) {
        printf("No se pudo crear la ventana: %s\n", SDL_GetError());
        return 0;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!*renderer) {
        printf("No se pudo crear el renderer: %s\n", SDL_GetError());
        return 0;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("No se pudo inicializar SDL_image: %s\n", IMG_GetError());
        return 0;
    }

    return 1;
}

SDL_Texture* cargarCarro(SDL_Renderer* renderer, const char* path) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        printf("Error cargando imagen %s: %s\n", path, IMG_GetError());
        return NULL;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return texture;
}

void dibujarEscenario(SDL_Renderer* renderer, SDL_Texture* carTexture) {
    // Fondo negro
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    int bloqueVerdeWidth = 60;
    int bloqueRojoWidth = 60;
    int calleWidth = 550;

    // Posiciones
    SDL_Rect bloqueVerdeIzq = {0, 0, bloqueVerdeWidth, WINDOW_HEIGHT};
    SDL_Rect bloqueRojoIzq = {bloqueVerdeWidth, 0, bloqueRojoWidth, WINDOW_HEIGHT};
    SDL_Rect calle = {bloqueVerdeWidth + bloqueRojoWidth, 0, calleWidth, WINDOW_HEIGHT};
    SDL_Rect bloqueRojoDer = {bloqueVerdeWidth + bloqueRojoWidth + calleWidth, 0, bloqueRojoWidth, WINDOW_HEIGHT};
    SDL_Rect bloqueVerdeDer = {bloqueVerdeWidth + bloqueRojoWidth + calleWidth + bloqueRojoWidth, 0, bloqueVerdeWidth, WINDOW_HEIGHT};

    // Bloque verde izquierdo
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &bloqueVerdeIzq);

    // Bloque rojo izquierdo
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &bloqueRojoIzq);

    // Calle blanca
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &calle);

    // Bloque rojo derecho
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &bloqueRojoDer);

    // Bloque verde derecho
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &bloqueVerdeDer);

    // Línea amarilla horizontal en medio de la calle
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    for (int x = calle.x; x < calle.x + calle.w; x += 40) {
        SDL_Rect linea = {x, WINDOW_HEIGHT / 2 - 5, 20, 10};
        SDL_RenderFillRect(renderer, &linea);
    }

    // Carritos sobre bloque verde izquierdo
    SDL_Rect carIzq1 = {10, 50, 40, 30};
    SDL_Rect carIzq2 = {10, 150, 40, 30};

    // Carritos sobre bloque verde derecho
    SDL_Rect carDer1 = {bloqueVerdeDer.x + 10, 50, 40, 30};
    SDL_Rect carDer2 = {bloqueVerdeDer.x + 10, 150, 40, 30};

    // Dibujar carritos
    SDL_RenderCopy(renderer, carTexture, NULL, &carIzq1);
    SDL_RenderCopy(renderer, carTexture, NULL, &carIzq2);
    SDL_RenderCopy(renderer, carTexture, NULL, &carDer1);
    SDL_RenderCopy(renderer, carTexture, NULL, &carDer2);

    // Mostrar en pantalla
    SDL_RenderPresent(renderer);
}





void cerrarInterfaz(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* carTexture) {
    SDL_DestroyTexture(carTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}
