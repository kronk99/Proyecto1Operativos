#include "interfaz.h"

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    if (!iniciarInterfaz(&window, &renderer)) {
        return 1;
    }

    SDL_Texture* carTexture = cargarCarro(renderer, "images/sport.png");
    if (!carTexture) {
        cerrarInterfaz(window, renderer, NULL);
        return 1;
    }

    int quit = 0;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }

        dibujarEscenario(renderer, carTexture);
        SDL_Delay(16);  // 60 FPS aprox
    }

    cerrarInterfaz(window, renderer, carTexture);

    return 0;
}
