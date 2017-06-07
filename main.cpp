#include <iostream>
#include <thread>
#include "reader.h"
#include "writer.h"
#include "data.h"


int main()
{
    int nClients;
    char hostname[16];

    Data sharedData;

    Semaphore *semaphore = new Semaphore();

    Writer writer( sharedData, *semaphore );

    Reader reader( sharedData, *semaphore );
    
    std::cout << "Digite o numero de clientes: ";
    std::cin >> nClients;

    int port[nClients];

    std::cout << "Digite porta do servidor: ";
    std::cin >> port[0];

    std::cout << "Digite hostname e porta do cliente: ";    
    std::cin >> hostname >> port[1];    

    std::thread tWriter = std::thread( &Writer::run, writer, port[0] );

    std::thread *tReader = new std::thread [nClients];

    for (int i = 0; i < nClients; i++) {
        tReader[i] = std::thread( &Reader::run, reader, hostname, port[1], false );
    }

    for (int i = 0; i < nClients; i++) {
        tReader[i].join();
    }

    tWriter.join();

    return 0;
}
