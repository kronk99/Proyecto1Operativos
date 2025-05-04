#include <stdio.h>
#include "headers/cars.h"
#include "headers/flow.h"
#include "headers/read.h"
#include "headers/CEmutex.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <linux/futex.h>
#include "interfaz.h"
#include <stdio.h>
#include <string.h>
int main() {
    struct CarList left, right; //crea la lista de left y rigth

    RoadData data; //crea el road

    readConfigurationFile("config.txt", &data); //configura el archivo

    // Inicializar las listas de carros
    initCarList(&left, 10);  // Lista para los carros de la izquierda
    initCarList(&right, 10); // Lista para los carros de la derecha

    for (int i = 0; i < data.carCount; i++) {
        if (data.cars[i].side == 0){
            addCar(&left, createCar(data.cars[i].id, 0, data.cars[i].speed, data.cars[i].priority, 
            data.cars[i].burstTime, data.cars[i].type));
        } else{
            addCar(&right, createCar(data.cars[i].id, 1, data.cars[i].speed, data.cars[i].priority, 
                data.cars[i].burstTime, data.cars[i].type));
        }
    }

    // Mostrar los carros en cada lado
    printf("Carros en el lado izquierdo:\n");
    for (int i = 0; i < left.count; i++) {
        printf("Carro ID: %d, Velocidad: %d, Prioridad: %d\n",
               left.cars[i].id, left.cars[i].speed, left.cars[i].priority);
    }

    printf("\nCarros en el lado derecho:\n");
    for (int i = 0; i < right.count; i++) {
        printf("Carro ID: %d, Velocidad: %d, Prioridad: %d\n",
               right.cars[i].id, right.cars[i].speed, right.cars[i].priority);
    }

    // Usar el algoritmo FIFO para hacer pasar los carros
    //aca se llama a equity que crea los hilos
    equity(&left, &right, data.parameterW);

    // Liberar memoria3
    freeCarList(&left);
    freeCarList(&right);

    return 0;
}
