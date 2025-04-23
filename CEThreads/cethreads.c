#define _GNU_SOURCE
#include "cethreads.h"
#include <stdio.h>
#include <sys/syscall.h>  // Para syscalls
#include <errno.h>


// Función envoltorio que ejecutará el hilo creado con clone.
// Esta función desempaqueta los argumentos y llama a la rutina real del hilo.
static int thread_wrapper(void *arg) {
    // Se recibe un puntero genérico que contiene un arreglo de dos punteros:
    // [0] -> función a ejecutar (start_routine)
    // [1] -> argumento para la función
    void **args = (void **)arg;
    void *(*start_routine)(void *) = args[0];  // Función que el hilo debe ejecutar
    void *routine_arg = args[1];               // Argumento para esa función

    // Ejecuta la función si no es NULL
    if (start_routine != NULL) {
        start_routine(routine_arg);
    }

    // Hay que poner la funcion aqui de eliminar hilo

    return 0;
}


// Función para crear un hilo utilizando clone() en lugar de pthreads.
// CEthread es una estructura personalizada que guarda el tid y la pila del hilo.
int CEthread_create(CEthread *thread, void *(*start_routine)(void *), void *arg) {
    // Reserva memoria para la pila del hilo con mmap. La pila crece hacia abajo (GROWSDOWN).
    thread->stack = mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE,
                         MAP_PRIVATE | MAP_ANONYMOUS | MAP_GROWSDOWN,
                         -1, 0);

    // Verifica que mmap no haya fallado
    if (thread->stack == MAP_FAILED) {
        perror("Error al asignar la pila");
        return -1;
    }

    // Prepara un arreglo de dos punteros para pasar a thread_wrapper:
    // el primero es la función que ejecutará el hilo, el segundo es su argumento
    void **args = (void **)malloc(2 * sizeof(void *));
    args[0] = start_routine;
    args[1] = arg;

    // Crea el hilo usando clone()
    // clone permite compartir memoria (CLONE_VM), archivos abiertos, señales, etc.
    // La pila se pasa al hilo desde el tope (por eso se suma STACK_SIZE)
    thread->tid = clone(thread_wrapper,
                        (char *)thread->stack + STACK_SIZE,
                        CLONE_VM | CLONE_FS | CLONE_FILES |
                        CLONE_SIGHAND | CLONE_THREAD | CLONE_SYSVSEM,
                        args);

    // Si clone falla, liberar la pila y los argumentos
    if (thread->tid == -1) {
        perror("Error al crear el hilo");
        munmap(thread->stack, STACK_SIZE);
        free(args);
        return -1;
    }

    // Hilo creado exitosamente
    return 0;
}
