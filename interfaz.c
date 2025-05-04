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

void dibujarEscenario(SDL_Renderer* renderer, SDL_Texture* carSport, SDL_Texture* carNormal, SDL_Texture* carEmergency, int largoCalle, int cantidadDeportivos, int cantidadNormales, int cantidadEmergencia) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    int bloqueVerdeWidth = 60;
    int bloqueRojoWidth = 60;
    int calleWidth = 600;  // ancho fijo visual de la calle

    // Dibujar bloques
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

    // Línea amarilla horizontal
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

    // === DIBUJAR CARROS ===
    int espaciadoVertical = 50;
    int offsetY = 50;
    int posY;

    // DIBUJAR CARROS IZQUIERDA
    int cuenta = 0;
    for (int i = 0; i < cantidadDeportivos; i++) {
        posY = offsetY + cuenta * espaciadoVertical;
        SDL_Rect car = {10, posY, 40, 30};
        SDL_RenderCopyEx(renderer, carSport, NULL, &car, 0, NULL, SDL_FLIP_HORIZONTAL);
        cuenta++;
    }
    for (int i = 0; i < cantidadNormales; i++) {
        posY = offsetY + cuenta * espaciadoVertical;
        SDL_Rect car = {10, posY, 40, 30};
        SDL_RenderCopyEx(renderer, carNormal, NULL, &car, 0, NULL, SDL_FLIP_HORIZONTAL);
        cuenta++;
    }
    for (int i = 0; i < cantidadEmergencia; i++) {
        posY = offsetY + cuenta * espaciadoVertical;
        SDL_Rect car = {10, posY, 40, 30};
        SDL_RenderCopyEx(renderer, carEmergency, NULL, &car, 0, NULL, SDL_FLIP_HORIZONTAL);
        cuenta++;
    }

    // DIBUJAR CARROS DERECHA
    cuenta = 0;
    for (int i = 0; i < cantidadDeportivos; i++) {
        posY = offsetY + cuenta * espaciadoVertical;
        SDL_Rect car = {bloqueVerdeDer.x + 10, posY, 40, 30};
        SDL_RenderCopy(renderer, carSport, NULL, &car);
        cuenta++;
    }
    for (int i = 0; i < cantidadNormales; i++) {
        posY = offsetY + cuenta * espaciadoVertical;
        SDL_Rect car = {bloqueVerdeDer.x + 10, posY, 40, 30};
        SDL_RenderCopy(renderer, carNormal, NULL, &car);
        cuenta++;
    }
    for (int i = 0; i < cantidadEmergencia; i++) {
        posY = offsetY + cuenta * espaciadoVertical;
        SDL_Rect car = {bloqueVerdeDer.x + 10, posY, 40, 30};
        SDL_RenderCopy(renderer, carEmergency, NULL, &car);
        cuenta++;
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
