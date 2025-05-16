#ifndef READ_H
#define READ_H

typedef struct {
    int largo_calle;
    int tipo_calendarizador;
    int tipo_flujo;
    int tiempo_letrero;
    int cantidad_letreros;
    int carros_deportivos;
    int carros_normales;
    int carros_emergencia;
} TrafficConfig;

void read(const char *filename, TrafficConfig *config);

#endif
