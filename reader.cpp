#include "reader.h"
#include <iostream>

Reader::Reader( Data &d, Semaphore &s ) : data(d), sem(s)
{
    front = data.rear;
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void Reader::entry( const bool &priority )
{    
    if( priority )
    {
        sem.P( sem.mutex );
        if( data.nClientsOn )
        {
            data.dWriters++;
            sem.V( sem.mutex );
            sem.P( sem.w ); // Writer (4K) semaphore
        }
        data.nWriters++;
        data.nClientsOn++;
        // Signal
        sem.V( sem.mutex );
    }
    else
    {
        sem.P( sem.mutex );
        if( data.nWriters )
        {
            data.dReaders++;
            sem.V( sem.mutex );
            sem.P( sem.r ); // Low priority readers semaphore
        }
        data.nReaders++;
        data.nClientsOn++;
        // Signal
        if( data.dReaders )
        {
            data.dReaders--;
            sem.V( sem.r );
        }
        else sem.V( sem.mutex );
    }
}

void Reader::exit( const bool &priority )
{
    if( priority )
    {
        sem.P( sem.mutex );
        data.nWriters--;
        data.nClientsOn--;
        // Signal
        if( data.dWriters )
        {
            data.dWriters--;
            sem.V( sem.w );
        }
        else if( data.dReaders )
        {
            data.dReaders--;
            sem.V( sem.r );
        }
        else sem.V( sem.mutex );
    }
    else
    {
        sem.P( sem.mutex );
        data.nReaders--;
        data.nClientsOn--;
        // Signal
        if( ( data.nClientsOn == 0 ) && ( data.dWriters ) )
        {
            data.dWriters--;
            sem.V( sem.w );
        }
        else sem.V( sem.mutex );
    }
}

void Reader::run( std::string sHostname, const unsigned int &port, const bool &priority, const unsigned int &i )
{
    id = i;
    int sock, n;
    unsigned int length;

    char hostname[16];
    for(unsigned i=0; i < sHostname.length(); i++)
    {
            hostname[i] = sHostname[i];
    }

    // A estrutura sockaddr_in contem um endereco de internet
    struct sockaddr_in server, from;

    // A estrutura hostent representa um host na Internet
    struct hostent *hp;

    // Cria um socket do tipo datagrama e retorna um descritor
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0) error("socket");

    // Preenche a estrutura "hp" a partir do nome da maquina ou de seu IP
    hp = gethostbyname(hostname);

    if (hp == 0) error("Unknown host");

    // Define a familia do endereco como do tipo Internet
    server.sin_family = AF_INET;
    // Copia o IP da estrutura "hp" para a estrutura "server"
    bcopy( (char *) hp->h_addr, (char *) &server.sin_addr, hp->h_length);
    // A funcao htons() converte o numero da porta para o padrao Little Endian.
    server.sin_port = htons(port);

    length = sizeof(struct sockaddr_in);

    char *aux = data.buffer;

    while(1)
    {
        // Protocolo de entrada (Leitores e Escritores)
        entry( priority );

        sem.P( sem.mutex2 );
        data.on[id] = true;
        data.cPaused[id] = false;
        sem.V( sem.mutex2 );

        while( data.Start[id] )
        {
            // Protocolo de entrada (Buffer limitado)
            sem.P( sem.mutex );
            if( data.dProducer )
            {
                data.dProducer = false;
                sem.V( sem.p );
            }
            sem.V( sem.mutex );

            sem.P( sem.mutex3 );
            if( front == data.rear )
            {
                data.dClientsOn++;
                sem.V( sem.mutex3 );
                sem.P( sem.sync );
            }
            else sem.V( sem.mutex3 );

            sem.P( sem.full[id] );
            // Envia dados pela rede. Parametros: socket, buffer que contem os dados,
            // tamanho do buffer, flags, endereco da maquina destino, tamanho da estrutura do endereco.
            // Retorna o numero de bytes enviados.
            n = sendto(sock, aux, BUFFER_SIZE, 0, (const struct sockaddr *) &server, length);
            if (n < 0) error("Sendto");
            // Protocolo de saída (Buffer limitado)
            sem.V( sem.empty[id] );

            front = ( front + BUFFER_SIZE ) % ( NBUFFERS * BUFFER_SIZE );
            aux = ( data.buffer + front );

            std::cout << "L" << id <<  std::endl;
        }
        // Protocolo de saída (Leitores e Escritores)
        exit( priority );

        sem.resetSem(id);

        sem.P( sem.mutex2 );
        data.on[id] = false;
        data.cPaused[id] = true;
        sem.V( sem.mutex2 );
        sem.P( sem.c[id] );        
    }

}

