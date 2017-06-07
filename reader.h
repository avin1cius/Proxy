#ifndef READER_H
#define READER_H

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

class Reader
{
    Data &data;
    Semaphore &sem;
    unsigned int front;

 public:
    Reader( Data &d, Semaphore &s );
    void signal( const bool & );
    void run( char *hostname, unsigned int port, bool priority );
};

#endif /* READER_H  */
