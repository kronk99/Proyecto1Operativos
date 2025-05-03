#ifndef CETHREADS_H
#define CETHREADS_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <linux/futex.h>
#include "CEthread_private.h"
#include "CEthreads_q.h"
#include "CEmutex.h"

typedef struct {
    pid_t tid;
} CEthread_t;

CEthread_private_t *__CEthread_selfptr();

int CEthread_create(void *(*start_routine)(void*), void *arg);
void CEthread_end();
int CEthread_join(CEthread_t target_thread, void **status);//CHECK THIS LATER
//ADDED MUTEX TO ARGS
void CEthread_yield();

#endif
