#ifndef CETHREAD_PRIVATE_H
#define CETHREAD_PRIVATE_H
#include <sys/types.h> 

typedef struct CEthread_private {
    pid_t tid;
    void *(*start_routine)(void *);
    void *arg;
    int state;
    struct CEthread_private *blockedForJoin;
    struct CEthread_private *next;
    void *returnValue;
} CEthread_private_t;

#endif
