#ifndef WRITER_H
#define WRITER_H

#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
//#include "pthread.h"
#include "semaphore.h"
#include "data.h"

class Writer
{
    Data &data;
    Semaphore &sem;
    int rear;
    
 public:
    Writer( Data& d, Semaphore& s );
    void run( unsigned int );
};

#endif /* WRITER_H  */
