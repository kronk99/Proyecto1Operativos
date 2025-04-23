#include "../include/CEthread_private.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <linux/futex.h>
#include "../include/CEthreads.h"

// se usa como punto de entrada del nuevo hilo
void CEthread_wrapper(void *(*start_routine)(void*), void *arg) {
    // obtiene el puntero al hilo actual (estructura privada del hilo)
    CEthread_private_t *self_ptr = (CEthread_private_t *)__CEthread_selfptr();

    // guarda la funcion y el argumento que va a ejecutar este hilo
    self_ptr->start_routine = start_routine;
    self_ptr->arg = arg;

    // marca el estado del hilo como corriendo
    self_ptr->state = 1; // RUNNING

    // ejecuta la funcion del hilo con el argumento dado
    start_routine(arg);

    // cuando termina la funcion, se llama a CEthread_end para limpiar
    CEthread_end();
}
///esta funcion actua como intermediaria cuando se lanza un nuevo hilo. en vez de llamar directamente a start_routine, se pasa esta funcion al sistema de hilos (por ejemplo con clone). 
// asi se garantiza que: el hilo siempre se inicializa correctamente (estructura self_ptr). se registra el estado como RUNNING. se ejecuta la funcion del usuario y al final se asegura 
// que se llama a CEthread_end() para terminar correctamente el hilo

