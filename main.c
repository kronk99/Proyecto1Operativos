#include "interfaz.h"
#include "Testing.h"  
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

    printf("Ingrese la cantidad de carros por lado (maximo 12): ");
    scanf("%d", &cantidadCarros);
    while (cantidadCarros < 1 || cantidadCarros > 12) {
        printf("Valor invalido. Ingrese entre 1 y 12: ");
        scanf("%d", &cantidadCarros);
    }

    // ======= SELECCIONAR TIPO DE CALENDARIZADOR =======
    int opcionCal;
    do {
        printf("Seleccione el tipo de calendarizador:\n");
        printf("  1. RR\n");
        printf("  2. Prioridad\n");
        printf("  3. SJF\n");
        printf("  4. FCFS\n");
        printf("  5. TiempoReal\n");
        printf("Opcion: ");
        scanf("%d", &opcionCal);

        if (opcionCal < 1 || opcionCal > 5) {
            printf("ERROR: Opcion invalida.\n");
        }
    } while (opcionCal < 1 || opcionCal > 5);

    switch (opcionCal) {
        case 1: strcpy(tipoCalendarizador, "RR"); break;
        case 2: strcpy(tipoCalendarizador, "Prioridad"); break;
        case 3: strcpy(tipoCalendarizador, "SJF"); break;
        case 4: strcpy(tipoCalendarizador, "FCFS"); break;
        case 5: strcpy(tipoCalendarizador, "TiempoReal"); break;
    }

    // ======= SELECCIONAR TIPO DE FLUJO =======
    int opcionFlujo;
    do {
        printf("Seleccione el tipo de flujo:\n");
        printf("  1. Equidad\n");
        printf("  2. Letrero\n");
        printf("  3. FIFO\n");
        printf("Opcion: ");
        scanf("%d", &opcionFlujo);

        if (opcionFlujo < 1 || opcionFlujo > 3) {
            printf("ERROR: Opcion invalida.\n");
        }
    } while (opcionFlujo < 1 || opcionFlujo > 3);

    switch (opcionFlujo) {
        case 1: strcpy(tipoFlujo, "Equidad"); break;
        case 2: strcpy(tipoFlujo, "Letrero"); break;
        case 3: strcpy(tipoFlujo, "FIFO"); break;
    }

    // ======= VALIDAR TIEMPO DEL LETRERO =======
    printf("Ingrese el tiempo del letrero (segundos, max 90): ");
    scanf("%d", &tiempoLetrero);
    while (tiempoLetrero < 0 || tiempoLetrero > 90) {
        printf("ERROR: Tiempo invalido. Ingrese entre 0 y 90 segundos: ");
        scanf("%d", &tiempoLetrero);
    }

    // ======= VALIDACION DE SUMA DE CARROS =======
    do {
        printf("Ingrese la cantidad inicial de carros deportivos por lado: ");
        scanf("%d", &cantidadDeportivos);

        printf("Ingrese la cantidad inicial de carros normales por lado:: ");
        scanf("%d", &cantidadNormales);

        printf("Ingrese la cantidad inicial de carros de emergencia por lado: ");
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

     // === CREAR LISTA DE CARROS ===
     Car* listaCarros[2*cantidadCarros];
     int id = 1;
     int i = 0;
 
     for (int j = 0; j < 2*cantidadDeportivos; j++, i++) {
         Car* car = malloc(sizeof(Car));
         car->id = id++;
         car->type = 1;
         car->direction = rand() % 2;
         listaCarros[i] = car;
     }
     for (int j = 0; j < 2*cantidadNormales; j++, i++) {
         Car* car = malloc(sizeof(Car));
         car->id = id++;
         car->type = 0;
         car->direction = rand() % 2;
         listaCarros[i] = car;
     }
     for (int j = 0; j < 2*cantidadEmergencia; j++, i++) {
         Car* car = malloc(sizeof(Car));
         car->id = id++;
         car->type = 2;
         car->direction = rand() % 2;
         listaCarros[i] = car;
     }

    // ======= INICIAR INTERFAZ GRAFICA =======
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

    int quit = 0;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = 1;
        }
        dibujarEscenario(renderer, carSport, carNormal, carEmergency, listaCarros, 2*cantidadCarros, largoCalle);
        SDL_Delay(16);
    }

    ejecutarSimulacion(listaCarros, 2*cantidadCarros, tipoCalendarizador);  // ejecutar testing

    cerrarInterfaz(window, renderer, carSport, carNormal, carEmergency);
    return 0;
}
