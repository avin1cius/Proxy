#ifndef SEMAPHORE_H
#define SEMAPHORE_H

//#include "pthread.h"
#include <semaphore.h>

class Semaphore
{
    
 public:
    sem_t mutex, pr, r, w;
    
    Semaphore()
    {
        sem_init( &mutex, 0, 1);
        sem_init( &hpr, 0, 0 );
        sem_init( &lpr, 0, 0);
        sem_init( &r, 0, 0);        
        sem_init( &w, 0, 0);
    }

    void P( sem_t &sem )
    {
        sem_wait( &sem );
    }

    void V( sem_t &sem )
    {
        sem_post( &sem );
    }
};

#endif /* SEMAPHORE_H  */
