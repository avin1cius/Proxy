#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <semaphore.h>
#include "data.h"
#include <iostream>

class Semaphore
{
    
 public:
    sem_t mutex, mutex2, mutex3, r, w, p, sync;
    sem_t empty[5], full[5], c[5];
    
    Semaphore()
    {
        sem_init( &empty[0], 0, NBUFFERS );
        sem_init( &empty[1], 0, NBUFFERS );
        sem_init( &empty[2], 0, NBUFFERS );
        sem_init( &empty[3], 0, NBUFFERS );
        sem_init( &empty[4], 0, NBUFFERS );

        sem_init( &full[0], 0, 0 );
        sem_init( &full[1], 0, 0 ) ;
        sem_init( &full[2], 0, 0 );
        sem_init( &full[3], 0, 0 );
        sem_init( &full[4], 0, 0 );

        sem_init( &c[0], 0, 0 );
        sem_init( &c[1], 0, 0 ) ;
        sem_init( &c[2], 0, 0 );
        sem_init( &c[3], 0, 0 );
        sem_init( &c[4], 0, 0 );

        sem_init( &mutex, 0, 1);
        sem_init( &mutex2, 0, 1);
        sem_init( &mutex3, 0, 1);
        sem_init( &r, 0, 0 );
        sem_init( &w, 0, 0 );
        sem_init( &p, 0, 0 );
        sem_init( &sync, 0, 0 );
    }

    void P( sem_t &sem )
    {
        sem_wait( &sem );
    }

    void V( sem_t &sem )
    {
        sem_post( &sem );
    }

    void resetSem( const int &id )
    {
        int aux, aux2;
        sem_getvalue( &empty[id], &aux );
        sem_getvalue( &full[id], &aux2 );

        while( aux != NBUFFERS )
        {
            sem_getvalue( &empty[id], &aux );
            //std::cout <<"Empty : " << aux<< std::endl;
            sem_post( &empty[id] );
        }
        std::cout<<"empty[" << id <<"]: " << aux <<std::endl;

        while( aux2 > 0 )
        {
            sem_getvalue( &full[id], &aux2 );
            //std::cout<< "Full : "<< aux2 << std::endl;
            sem_wait( &full[id] );
       }
       std::cout<<"full[" << id <<"]: " << aux2 <<std::endl;
    }
};

#endif /* SEMAPHORE_H  */
