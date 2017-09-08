#ifndef PRODUCER_H
#define PRODUCER_H

#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include "semaphores.h"
#include "data.h"

class Producer
{
    Data &data;
    //Semaphore &sem;
    
 public:
    Semaphore &sem;

    Producer( Data &d, Semaphore &s );
    void entry();
    void exit();
    void run( const char *hostname, const unsigned int & );
};

#endif /* PRODUCER_H  */
