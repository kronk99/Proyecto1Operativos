#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read.h"

void read(const char *filename, TrafficConfig *config) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("No se pudo abrir el archivo");
        exit(EXIT_FAILURE);
    }

    char linea[100];

    while (fgets(linea, sizeof(linea), file)) {
        if (strstr(linea, "Largo de la calle:") != NULL)
            sscanf(linea, "Largo de la calle: %d", &config->largo_calle);
        else if (strstr(linea, "Tipo de calendarizador:") != NULL)
            sscanf(linea, "Tipo de calendarizador: %d", &config->tipo_calendarizador);
        else if (strstr(linea, "Tipo de flujo:") != NULL)
            sscanf(linea, "Tipo de flujo: %d", &config->tipo_flujo);
        else if (strstr(linea, "Tiempo del letrero:") != NULL)
            sscanf(linea, "Tiempo del letrero: %d", &config->tiempo_letrero);
        else if (strstr(linea, "Cantidad de letrero:") != NULL)
            sscanf(linea, "Cantidad de letrero: %d", &config->cantidad_letreros);
        else if (strstr(linea, "Cantidad inicial carros deportivos:") != NULL)
            sscanf(linea, "Cantidad inicial carros deportivos: %d", &config->carros_deportivos);
        else if (strstr(linea, "Cantidad inicial carros normales:") != NULL)
            sscanf(linea, "Cantidad inicial carros normales: %d", &config->carros_normales);
        else if (strstr(linea, "Cantidad inicial carros emergencia:") != NULL)
            sscanf(linea, "Cantidad inicial carros emergencia: %d", &config->carros_emergencia);
    }

    fclose(file);
}
