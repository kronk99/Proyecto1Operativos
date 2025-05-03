#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <linux/futex.h>
#include "../headers/futex.h"

// funcion para inicializar un futex con un valor dado
void futex_init(int *futex, int value) {
    printf("futex_init called\n");
    // asigna el valor inicial al futex
    *futex = value;
}

// funcion para hacer "down" = esperar el futex
void futex_down(int *futex) {
    printf("futex_down called\n");
    // mientras el valor del futex no sea cero, espera con syscall
    while (*futex != 0) {
        // bloquea el hilo actual hasta que el futex sea liberado
        syscall(SYS_futex, futex, FUTEX_WAIT, 0, NULL, NULL, 0);
    }
    // cuando futex vale cero, continua la ejecucion
}

// funcion para hacer "up" = liberar el futex
void futex_up(int *futex) {
    printf("futex_up called\n");
    // asigna el valor 1, indicando que el recurso esta disponible
    *futex = 1;
    // despierta un hilo que este esperando este futex
    syscall(SYS_futex, futex, FUTEX_WAKE, 1, NULL, NULL, 0);
}
