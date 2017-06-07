#ifndef DATA_H
#define DATA_H

#define BUFFER_SIZE 1316
#define NBUFFERS 48

struct Data
{
    char buffer[NBUFFERS*BUFFER_SIZE];
    int nReaders, nPriorityReaders;
    int dReaders, dPriorityReaders;
    int front =0, rear = 0;
};

#endif /* DATA_H  */
