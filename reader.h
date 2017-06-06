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

class Reader
{
    char &buf;
    bool priority;
    int executionTime;

 public:
    void run();
};

#endif /* READER_H  */
