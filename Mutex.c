#include <stdatomic.h>
#include <linux/futex.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <errno.h>
typedef struct{
    atomic_int flag; //bandera del mutex , 0 esta libre 1 esta ocupado
}mutex_flag;
void mutex_lock(mutex_flag *mutex){
    int expected_Value =0;
    //verifica que el mutex este tomado
    if (!atomic_compare_exchange_strong(&mutex->flag, &expected_Value, 1)) {
        //si el valordel mutex flag es 0 , se pasa a 1, pero si el exchange strong falla
        // Ya estaba ocupado, ahora esperar con futex
        while (1) {
            expected_Value = 0;
            //  vuelve a probar el cambio para tomar el hilo
            if (atomic_compare_exchange_strong(&mutex->flag, &expected_Value, 1))
                break;
            // Dormir hasta que el lock cambie
            //llamada al kernel de linux, si el valor del mutex flag es 1, 
            //mande el hilo a dormir, se mueve a la lista de espera de flutex
            //lo maneja internamente la syscall, duerme el hilo con flutex_wait hasta que otro hilo
            //lo despierte, lo "encola"y cambia al estado sleeping
            syscall(SYS_futex, &mutex->flag, FUTEX_WAIT, 1, NULL, NULL, 0);
        }
    }
}
void mutex_unlock(mutex_flag *mutex) {
    atomic_store(&mutex->flag ,0);
    // Despertar a uno de los hilos esperando (si hay), lo pasa al estaado activo
    syscall(SYS_futex, &mutex->flag, FUTEX_WAKE, 1, NULL, NULL, 0);
}
void mutex_init(mutex_flag *mutex) { //inicializacion del mutex
    atomic_init(&mutex->flag, 0);
}