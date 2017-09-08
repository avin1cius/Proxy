#include "producer.h"
#include <iostream>

Producer::Producer( Data &d, Semaphore &s ) : data(d), sem(s){}

void error(const char *msg);

void Producer::entry()
{
    for(unsigned i = 0; i < data.nClients; i++)
    {
        sem.P( sem.mutex2 );
        if( !data.cPaused[i] && data.on[i] )
        {
            sem.V( sem.mutex2 );
            sem.P( sem.empty[i] );
        }
        else sem.V( sem.mutex2 );
    }    

}

void Producer::exit()
{
    for(unsigned i=0; i < data.nClients; i++)
    {
        sem.P( sem.mutex2 );
        if( !data.cPaused[i] && data.on[i] )
        {
            sem.V( sem.mutex2 );
            sem.V( sem.full[i] );
        }
        else sem.V( sem.mutex2 );
    }
}

void Producer::run(  const char *hostname, const unsigned int &port )
{
    int sock, length, n;
    socklen_t fromlen;

    // A estrutura sockaddr_in contem um endereco de internet
    struct sockaddr_in server, from;

    // A estrutura hostent representa um host na Internet
    struct hostent *hp;
    
    // Cria um socket do tipo datagrama e retorna um descritor
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0) error("Opening socket");

    length = sizeof(server);

    // Preenche a estrutura "hp" a partir do nome da maquina ou de seu IP
    hp = gethostbyname(hostname);

    if (hp == 0) error("Unknown host");

    // Zera a estrutura "server"
    bzero(&server, length);
    // Define a familia do endereco como do tipo Internet
    server.sin_family = AF_INET;
    // Copia o IP da estrutura "hp" para a estrutura "server"
    bcopy( (char *) hp->h_addr, (char *) &server.sin_addr, hp->h_length);
    // A funcao htons() converte o numero da porta para o padrao Little Endian.
    server.sin_port = htons(port);

    // Associa um socket a um endereco
    if (bind(sock, (struct sockaddr *) &server, length) < 0)
        error("binding");

    fromlen = sizeof(struct sockaddr_in);

    char *aux = data.buffer;

    while (1)
    {
        sem.P( sem.mutex );
        if( !data.nClientsOn )
        {
            data.dProducer = true;
            sem.V( sem.mutex );
            sem.P( sem.p );
        }
        else sem.V( sem.mutex );



        // Protocolo de entrada (Buffer limitado)
        entry();        
        std::cout << "P" << std::endl;
        // A funcao recvfrom() bloqueia o programa ate que um pacote seja lido.
        // Quando isso acontece, o pacote e' armazenado em um buffer passado por
        // parametro. Parametros: socket, buffer, tamanho do buffer, flags,
        // endereco da maquina que enviou o pacote, tamanho da estrutura do endereco.
        // Retorna o numero de bytes recebidos.
        n = recvfrom(sock, aux, BUFFER_SIZE, 0, (struct sockaddr *) &from, &fromlen);
        if (n < 0) error("recvfrom");

        sem.P( sem.mutex3 );
        data.rear = ( data.rear + BUFFER_SIZE ) % ( NBUFFERS * BUFFER_SIZE );

        if( data.dClientsOn )
        {
            data.dClientsOn--;
            sem.V( sem.sync );
        }
        sem.V( sem.mutex3 );

        // Protocolo de saida (Buffer limitado)
        exit();

        aux = ( data.buffer + data.rear );
    }
 }
