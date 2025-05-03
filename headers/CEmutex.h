#ifndef CEMUTEX_H
#define CEMUTEX_H

#include <stdatomic.h>
#include <linux/futex.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>

typedef struct {
    atomic_int value; // 0 = libre, 1 = ocupado
}CEmutex_t;

void CEmutex_init(CEmutex_t *mutex);
void CEmutex_destroy(CEmutex_t *mutex);
void CEmutex_lock(CEmutex_t *mutex);
void CEmutex_unlock(CEmutex_t *mutex);

#endif
