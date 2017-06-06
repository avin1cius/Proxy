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

class Writer
{
    char &buf
    
 public:
    void run();
};

#endif /* WRITER_H  */
