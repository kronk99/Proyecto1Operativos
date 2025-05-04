#include "interfaz.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
    int largoCalle;
    int cantidadCarros;
    char tipoCalendarizador[20];
    char tipoFlujo[20];
    int tiempoLetrero;
    int cantidadDeportivos;
    int cantidadNormales;
    int cantidadEmergencia;

    // ======= PEDIR DATOS AL USUARIO =======
    printf("Ingrese el largo de la calle (100 - 600): ");
    scanf("%d", &largoCalle);
    while (largoCalle < 100 || largoCalle > 600) {
        printf("Valor invalido. Ingrese entre 100 y 600: ");
        scanf("%d", &largoCalle);
    }

    printf("Ingrese la cantidad de carros (maximo 12): ");
    scanf("%d", &cantidadCarros);
    while (cantidadCarros < 1 || cantidadCarros > 12) {
        printf("Valor invalido. Ingrese entre 1 y 12: ");
        scanf("%d", &cantidadCarros);
    }

    printf("Ingrese el tipo de calendarizador (RR, Prioridad, SJF, FCFS, TiempoReal): ");
    scanf("%s", tipoCalendarizador);

    printf("Ingrese el tipo de flujo (Equidad, Letrero, FIFO): ");
    scanf("%s", tipoFlujo);

    printf("Ingrese el tiempo del letrero (segundos): ");
    scanf("%d", &tiempoLetrero);

    // ======= VALIDACION DE SUMA DE CARROS =======
    do {
        printf("Ingrese la cantidad inicial de carros deportivos: ");
        scanf("%d", &cantidadDeportivos);

        printf("Ingrese la cantidad inicial de carros normales: ");
        scanf("%d", &cantidadNormales);

        printf("Ingrese la cantidad inicial de carros de emergencia: ");
        scanf("%d", &cantidadEmergencia);

        if (cantidadDeportivos < 0 || cantidadNormales < 0 || cantidadEmergencia < 0) {
            printf("ERROR: Ninguna cantidad puede ser negativa.\n");
        } else if (cantidadDeportivos + cantidadNormales + cantidadEmergencia != cantidadCarros) {
            printf("ERROR: La suma de deportivos, normales y emergencia debe ser igual a la cantidad total de carros (%d).\n", cantidadCarros);
        }

    } while (cantidadDeportivos < 0 || cantidadNormales < 0 || cantidadEmergencia < 0 ||
             cantidadDeportivos + cantidadNormales + cantidadEmergencia != cantidadCarros);

    // ======= GUARDAR DATOS EN ARCHIVO =======
    FILE* archivo = fopen("configuracion.txt", "w");
    if (archivo == NULL) {
        printf("Error al abrir archivo para escribir.\n");
        return 1;
    }

    fprintf(archivo, "Largo de la calle: %d\n", largoCalle);
    fprintf(archivo, "Cantidad de carros: %d\n", cantidadCarros);
    fprintf(archivo, "Tipo de calendarizador: %s\n", tipoCalendarizador);
    fprintf(archivo, "Tipo de flujo: %s\n", tipoFlujo);
    fprintf(archivo, "Tiempo del letrero: %d\n", tiempoLetrero);
    fprintf(archivo, "Cantidad inicial carros deportivos: %d\n", cantidadDeportivos);
    fprintf(archivo, "Cantidad inicial carros normales: %d\n", cantidadNormales);
    fprintf(archivo, "Cantidad inicial carros emergencia: %d\n", cantidadEmergencia);

    fclose(archivo);
    printf("Datos guardados en configuracion.txt.\n");

    // ======= INICIAR INTERFAZ GRAFICA =======
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    if (!iniciarInterfaz(&window, &renderer)) {
        return 1;
    }

    SDL_Texture* carSport = cargarCarro(renderer, "images/sport.png");
    SDL_Texture* carNormal = cargarCarro(renderer, "images/normal.png");
    SDL_Texture* carEmergency = cargarCarro(renderer, "images/emergency.png");

    if (!carSport || !carNormal || !carEmergency) {
        cerrarInterfaz(window, renderer, carSport, carNormal, carEmergency);
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

        dibujarEscenario(renderer, carSport, carNormal, carEmergency, largoCalle, cantidadDeportivos, cantidadNormales, cantidadEmergencia);
        SDL_Delay(16);
    }

    cerrarInterfaz(window, renderer, carSport, carNormal, carEmergency);

    return 0;
}
