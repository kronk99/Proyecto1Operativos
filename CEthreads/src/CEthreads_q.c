#include "../include/CEthread_private.h"
#include <stddef.h>
#include "../include/CEthreads_q.h"

// cabeza de la lista enlazada de hilos activos
CEthread_private_t *CEthread_q_head = NULL;

// funcion que busca un hilo en la lista por su identificador tid
CEthread_private_t *CEthread_q_search(unsigned long tid) {
    // comienza desde la cabeza de la lista
    CEthread_private_t *current = CEthread_q_head;

    // recorre la lista mientras no sea el final
    while (current != NULL) {
        // si encuentra el hilo con el tid buscado, lo retorna
        if (current->tid == tid) {
            return current;
        }
        // avanza al siguiente nodo de la lista
        current = current->next;
    }

    // si no encuentra el hilo, retorna null
    return NULL;
}
//Para CEthread_join pq permite encontrar el hilo objetivo al que se quiere esperar. tambien para modificar el estado de un hilo en especifico

