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

void Reader::signal( const bool &priority )
{
    sem.P( sem.mutex );
    if( priority && (!data.nPriorityReaders) )
    { 
        data.nPriorityReaders++;
        sem.V( sem.mutex );
    }
    else if( priority )
    {
        data.dPriorityReaders++;
        sem.V( sem.mutex );
        sem.P( sem.hpr ); // High priority readers semaphore
        data.nPriorityReaders++;            
    }
    else if( data.nPriorityReaders )
    {
        data.dReaders++;
        sem.V( sem.mutex );
        sem.P( sem.lpr ); // Low priority readers semaphore
        data.nReaders++;
    }
    else{
        data.nReaders++;
        if( data.dReaders )
        {
            data.dReaders--;
            sem.V( sem.lpr );
        }
        else sem.V( sem.mutex );
    }
}

void Reader::run( char *hostname, unsigned int port, bool priority )
{
    int sock, n;
    unsigned int length;

    // A estrutura sockaddr_in contem um endereco de internet
    struct sockaddr_in server, from;

    // A estrutura hostent representa um host na Internet
    struct hostent *hp;

    // Cria um socket do tipo datagrama e retorna um descritor
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0) error("socket");

    // Define a familia do endereco como do tipo Internet
    server.sin_family = AF_INET;

    // Preenche a estrutura "hp" a partir do nome da maquina ou de seu IP
    hp = gethostbyname(hostname);

    if (hp == 0) error("Unknown host");

    // Copia o IP da estrutura "hp" para a estrutura "server"
    bcopy( (char *) hp->h_addr, (char *) &server.sin_addr, hp->h_length);
    // A funcao htons() converte o numero da porta para o padrao Little Endian.
    server.sin_port = htons(port);

    length = sizeof(struct sockaddr_in);

    char *aux = data.buffer;

    while(1) //START 
    { 
        // Dar acesso exclusivo ao leitor com prioridade
        signal( priority );

        // Envia dados pela rede. Parametros: socket, buffer que contem os dados,
        // tamanho do buffer, flags, endereco da maquina destino, tamanho da estrutura do endereco.
        // Retorna o numero de bytes enviados.
        n = sendto(sock, aux, BUFFER_SIZE, 0, (const struct sockaddr *) &server, length);

        if (n < 0) error("Sendto");

        sem.P( sem.mutex );
        data.sync[front]++;
        if( data.sync[front] == data.nClients )
        {
            if( data.dWriter ) { data.dWriter = false; sem.V( sem.w ); }
        }
        sem.V( sem.mutex );
        
        front = ( front + BUFFER_SIZE ) % ( NBUFFERS * BUFFER_SIZE );
        
        aux = ( data.buffer + front );

        sem.P( sem.mutex );
        if( priority ) 
        {
            data.nPriorityReaders--;
            if( data.dPriorityReaders )
            {
                data.dPriorityReaders--;
                sem.V( sem.hpr);
            }
            else if( data.dReaders ) sem.V( sem.lpr );
            else sem.V( sem.mutex );
        }
        else { data.nReaders--; sem.V( sem.mutex ); }
    }    
}
