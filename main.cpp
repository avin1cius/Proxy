#include <iostream>
#include <thread>
#include "reader.h"
#include "writer.h"
#include "data.h"


int main()
{
    int nClients, serverPort;
    char hostname[16];

    Data sharedData;

    Semaphore *semaphore = new Semaphore();

    Writer writer( sharedData, *semaphore );

    Reader reader( sharedData, *semaphore );

    std::cout << "Digite porta do servidor: ";
    std::cin >> serverPort;
    
    std::cout << "Digite o numero de clientes: ";
    std::cin >> nClients;

    int port[nClients];

    for(int i=0; i<nClients; i++)
    {
        std::cout << "Digite hostname e porta do cliente " << i+1 << ": ";    
        std::cin >> hostname >> port[i];    
    }    

    std::thread tWriter = std::thread( &Writer::run, writer, serverPort );

    std::thread *tReader = new std::thread [nClients];

    for (int i = 0; i < nClients; i++) {
        tReader[i] = std::thread( &Reader::run, reader, hostname, port[i], false );
    }

    for (int i = 0; i < nClients; i++) {
        tReader[i].join();
    }

    tWriter.join();

    return 0;
}
