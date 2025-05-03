#include "../headers/CEthreads.h"
#include "../headers/CEthread_private.h"
#include "../headers/CEmutex.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <linux/futex.h>
#include "../headers/CEthreads_q.h"
#include <sys/types.h>


// variable global EXTERNA que apunta a la cabeza de la lista enlazada de hilos
extern CEthread_private_t *CEthread_q_head;

// retorna el puntero al hilo actual
CEthread_private_t *__CEthread_selfptr() {
    static CEthread_private_t main_thread; // estatica para el hilo principal
    static int initialized = 0;            // bandera para asegurarse de que se inicialice SOLO una vez

    if (!initialized) {
        // si no se ha inicializado, se asigna la informacion del hilo principal
        main_thread.tid = syscall(SYS_gettid);     // se obtiene el id del hilo actual
        main_thread.start_routine = NULL;          // no tiene funcion asociada porque es el main
        main_thread.arg = NULL;                    // tampoco tiene argumentos
        main_thread.state = 1;                     // estado 1 indica que esta corriendo
        main_thread.blockedForJoin = NULL;         // no esta esperando a ningun otro hilo
        main_thread.next = NULL;                   // no hay siguiente hilo aun
        main_thread.returnValue = NULL;            // no tiene valor de retorno
        initialized = 1;                           // se marca como inicializado para futuras llamadas
    }

    return &main_thread; // retorna el puntero al hilo principal
}

// funcion publica para crear un nuevo hilo
int CEthread_create(void *(*start_routine)(void*), void *arg) {
    // se utiliza clone para crear un hilo compartiendo memoria y otros recursos
    pid_t pid = syscall(SYS_clone, 0x00000011, 0); // flags CLONE_VM | CLONE_FS | CLONE_FILES

    if (pid < 0) {
        // si clone falla, retorna -1 como error
        return -1;
    } else if (pid == 0) {
        // si el retorno es 0, estamos en el hilo hijo
        CEthread_private_t *self_ptr = __CEthread_selfptr(); // obtenemos la estructura del hilo
        self_ptr->start_routine = start_routine; // guard la funcion que debe ejecutar
        self_ptr->arg = arg;                     // guarda los argumentos
        self_ptr->state = 1;                     // lo marca como corriendo
        void *retval = start_routine(arg);       // ejecuta la funcion con los argumentos
        self_ptr->returnValue = retval;          // guarda el valor de retorno
        CEthread_end();                          // cuandeo termina llama la funcion de finalizacion
        return 0;                                // para evitar el warning, no ejecuta. 
    } else {
        // si el valor es mayor que cero, estamos en el hilo padre
        // se reserva memoria para representar al nuevo hilo
        CEthread_private_t *new_thread = malloc(sizeof(CEthread_private_t));
        new_thread->tid = pid;                   // se guarda el id del nuevo hilo
        new_thread->start_routine = start_routine;
        new_thread->arg = arg;
        new_thread->state = 1;                   // estado corriendo
        new_thread->returnValue = NULL;          // aun no hay valor de retorno

        // se mete el nuevo hilo al principio de la lista enlazada
        new_thread->next = CEthread_q_head;
        CEthread_q_head = new_thread;

        return pid; // retorna el pid del nuevo hilo
    }
}

// para terminar un hilo
void CEthread_end() {
    CEthread_private_t *self_ptr = __CEthread_selfptr(); // obtiene el hilo actual
    self_ptr->state = 2;                                // lo marca como terminado
    syscall(SYS_exit, 0);                               // hace exit sin retornar al padre
}

//  para esperar a que otro hilo termine
int CEthread_join(CEthread_t target_thread, void **status) {
    CEthread_private_t *target, *self_ptr;
    self_ptr = __CEthread_selfptr(); // obtiene el hilo actual

    // busca el hilo objetivo en la lista por su tHREAD id
    target = (CEthread_private_t *)CEthread_q_search(target_thread.tid);

    if (target->state == 2) {
        // si el hilo ya termino, recoge el valor de retorno
        *status = target->returnValue;

        // se elimina el hilo de la lista
        if (target == CEthread_q_head) {
            CEthread_q_head = target->next;
        } else {
            CEthread_private_t *current = CEthread_q_head;
            while (current->next != target) {
                current = current->next;
            }
            current->next = target->next;
        }

        free(target); // libera la memoria
        return 0;
    } else {
        // si el hilo aun no ha terminado
        self_ptr->state = 3; // se pone en estado bloqueado
        target->blockedForJoin = (struct CEthread_private *)self_ptr; // el hilo actual espera al objetivo

        // se duerme el hilo hasta que el otro termine
        syscall(SYS_futex, &target->tid, FUTEX_WAIT);

        // al despertar, se recoge el valor de retorno
        *status = target->returnValue;

        // eliminA el hilo de la lista
        if (target == CEthread_q_head) {
            CEthread_q_head = target->next;
        } else {
            CEthread_private_t *current = CEthread_q_head;
            while (current->next != target) {
                current = current->next;
            }
            current->next = target->next;
        }

        free(target); // libera la memoria del hilo ya finalizado
        return 0;
    }
}

// funcion para ceder el control del cpu a otros hilos
void CEthread_yield() {
    syscall(SYS_sched_yield); // llamada al sistema para rendir el uso del cpu voluntariamente
}