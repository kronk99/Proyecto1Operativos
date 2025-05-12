#include "../include/CEmutex.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <linux/futex.h>
#include <errno.h>

// define la estructura del mutex como en el estándar moderno
//typedef struct {
  //  atomic_int value; // 0 = libre, 1 = ocupado
//} 

//CEmutex_t;

// inicializa el mutex a 0 (libre)
void CEmutex_init(CEmutex_t *mutex) {
    atomic_init(&mutex->value, 0);
}

// destruye el mutex (lo deja libre de nuevo)
void CEmutex_destroy(CEmutex_t *mutex) {
    atomic_store(&mutex->value, 0);
}

// bloquea el mutex (versión moderna con lógica robusta)
void CEmutex_lock(CEmutex_t *mutex) {
    int expected = 0;

    // intenta tomar el lock inicialmente
    if (!atomic_compare_exchange_strong(&mutex->value, &expected, 1)) {
        // si ya está ocupado, entra a ciclo de espera con retry
        while (1) {
            expected = 0;
            // reintenta tomar el lock
            if (atomic_compare_exchange_strong(&mutex->value, &expected, 1))
                break;

            // duerme si sigue ocupado
            syscall(SYS_futex, &mutex->value, FUTEX_WAIT, 1, NULL, NULL, 0);
        }
    }
}

// desbloquea el mutex (libera y despierta un hilo)
void CEmutex_unlock(CEmutex_t *mutex) {
    atomic_store(&mutex->value, 0);
    printf("coloque el valor del mutex en 0");
    // despierta a uno de los hilos en espera
    syscall(SYS_futex, &mutex->value, FUTEX_WAKE, 1, NULL, NULL, 0);
}
