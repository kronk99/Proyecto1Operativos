#include <stdio.h>
#include "headers/cars.h"
#include "headers/flow.h"

int main() {
    struct CarList left, right;

    // Inicializar las listas de carros
    initCarList(&left, 10);  // Lista para los carros de la izquierda
    initCarList(&right, 10); // Lista para los carros de la derecha

    // Crear y agregar 5 carros al lado izquierdo (con ID 1 a 5)
    addCar(&left, createCar(1, 0, 2, 1, 3, 0));  // Carro 1
    addCar(&left, createCar(2, 0, 3, 2, 2, 0));  // Carro 2
    addCar(&left, createCar(3, 0, 1, 1, 1, 0));  // Carro 3
    addCar(&left, createCar(4, 0, 4, 3, 4, 0));  // Carro 4
    addCar(&left, createCar(5, 0, 2, 1, 2, 0));  // Carro 5

    // Crear y agregar 5 carros al lado derecho (con ID 6 a 10)
    addCar(&right, createCar(6, 1, 3, 2, 2, 0));  // Carro 6
    addCar(&right, createCar(7, 1, 2, 1, 1, 0));  // Carro 7
    addCar(&right, createCar(8, 1, 1, 2, 3, 0));  // Carro 8
    addCar(&right, createCar(9, 1, 4, 3, 4, 0));  // Carro 9
    addCar(&right, createCar(10, 1, 2, 1, 2, 0)); // Carro 10

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
    fifo(&left, &right);

    // Liberar memoria
    freeCarList(&left);
    freeCarList(&right);

    return 0;
}
