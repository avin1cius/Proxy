#ifndef DATA_H
#define DATA_H

#define BUFFER_SIZE 1316
#define NBUFFERS 1024

struct Data
{
    char buffer[NBUFFERS*BUFFER_SIZE];
    bool on[5] = { false } ;
    bool cPaused[5] = { false };
    bool Start[5] = { false };
    int nClientsOn = 0, nWriters = 0, nReaders = 0;
    int dClientsOn = 0, dWriters = 0, dReaders = 0;
    bool dProducer = false;
    unsigned int nClients = 5, front = 0, rear = 0;
};

#endif /* DATA_H  */
