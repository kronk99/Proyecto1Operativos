#include "interfaz.h"
#include <stdio.h>
#include "Threads/Car.h"
#include "Calendarizador/ReadyQueue.h"


SDL_Renderer* globalRenderer = NULL;

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
    globalRenderer = *renderer;
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("No se pudo inicializar SDL_image: %s\n", IMG_GetError());
        return 0;
    }

    return 1;
}
//ESTO PINTA EL CARRO EN UNAS COORDNEADAS ESPECIFICADAS
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

void dibujarEscenario(SDL_Renderer* renderer, ReadyQueue* queueRight, ReadyQueue* queueLeft,int largoCalle) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    int bloqueVerdeWidth = 60;
    int bloqueRojoWidth = 60;
    int calleWidth = 600;

    SDL_Rect bloqueVerdeIzq = {0, 0, bloqueVerdeWidth, WINDOW_HEIGHT};
    SDL_Rect bloqueRojoIzq = {bloqueVerdeWidth, 0, bloqueRojoWidth, WINDOW_HEIGHT};
    SDL_Rect calle = {bloqueVerdeWidth + bloqueRojoWidth, 0, calleWidth, WINDOW_HEIGHT};
    SDL_Rect bloqueRojoDer = {bloqueVerdeWidth + bloqueRojoWidth + calleWidth, 0, bloqueRojoWidth, WINDOW_HEIGHT};
    SDL_Rect bloqueVerdeDer = {bloqueVerdeWidth + bloqueRojoWidth + calleWidth + bloqueRojoWidth, 0, bloqueVerdeWidth, WINDOW_HEIGHT};

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &bloqueVerdeIzq);
    SDL_RenderFillRect(renderer, &bloqueVerdeDer);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &bloqueRojoIzq);
    SDL_RenderFillRect(renderer, &bloqueRojoDer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &calle);

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    int cantidadLineas = largoCalle / 10;
    if (cantidadLineas < 1) cantidadLineas = 1;
    int espacioEntreLineas = calle.w / cantidadLineas;
    int anchoLinea = espacioEntreLineas / 2;
    for (int i = 0; i < cantidadLineas; i++) {
        int xLinea = calle.x + i * espacioEntreLineas;
        SDL_Rect linea = {xLinea, WINDOW_HEIGHT / 2 - 5, anchoLinea, 10};
        SDL_RenderFillRect(renderer, &linea);
    }

    // === DIBUJAR CADA CARRO SEGÚN SU TIPO ===
    int espaciadoVertical = 50; // espacio vertical entre carros
    int offsetY = 50;           // separación inicial desde arriba

    int contadorIzquierda = 0;  // cuántos carros van por el lado izquierdo
    int contadorDerecha = 0;    // cuántos carros van por el lado derecho

        // Dibujar carros de la izquierda
    queue_reset(queueLeft);
    Car* car;
    while ((car = queue_next(queueLeft)) != NULL) {
        SDL_Texture* textura = NULL;

        textura = car->carTexture;

        int posY = offsetY + contadorIzquierda * espaciadoVertical;
        SDL_Rect carRect = {10, posY, 40, 30}; // lado izquierdo

        SDL_RenderCopyEx(renderer, textura, NULL, &carRect, 0, NULL, SDL_FLIP_HORIZONTAL);
        contadorIzquierda++;
    }

        // Dibujar carros de la derecha
    queue_reset(queueRight);
    while ((car = queue_next(queueRight)) != NULL) {
        SDL_Texture* textura = NULL;
        
        textura = car->carTexture;

        int posY = offsetY + contadorDerecha * espaciadoVertical;
        SDL_Rect carRect = {bloqueVerdeDer.x + 10, posY, 40, 30}; // lado derecho

        SDL_RenderCopy(renderer, textura, NULL, &carRect);
        contadorDerecha++;
    }



    SDL_RenderPresent(renderer);
}

void cerrarInterfaz(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* carSport, SDL_Texture* carNormal, SDL_Texture* carEmergency) {
    if (carSport) SDL_DestroyTexture(carSport);
    if (carNormal) SDL_DestroyTexture(carNormal);
    if (carEmergency) SDL_DestroyTexture(carEmergency);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}