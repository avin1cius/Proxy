#include <iostream>
#include "reader.h"
#include "writer.h"

#define BUFFER_SIZE 1316
#define NBUFFERS 48

using namespace std;

int main() {

    char buf[NBUFFERS*BUFFER_SIZE];

    std::cout << "Digite o numero de clientes" << std::endl;
    std::cin >> nClients;

    Writer writer( *buf );

    Reader reader( *buf );

    std::thread tWriter = std::thread( &Carro::run, writer, port, ip );

    std::thread *tReader = new std::thread [nClients];

    for (int i = 0; i < nClients; i++) {
        tReader[i] = std::thread( &Reader::run, reader, port, ip );
    }

    for (int i = 0; i < nClients; i++) {
        tReader[i].join();
    }

    tCarro.join();

    delete carro;

    return 0;
}
