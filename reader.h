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
#include "semaphores.h"
#include "data.h"

class Reader
{
    Data &data;
    Semaphore &sem;
    unsigned int front, id;

 public:
    Reader( Data &d, Semaphore &s );
    void entry( const bool & );
    void exit( const bool & );
    void run( const std::string, const unsigned int &, const bool &, const unsigned int & );
};

#endif /* READER_H  */
