#include "writer.h"

Writer::Writer( char &b) : buf(b){}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void Writer::run(int argc, char *argv[])
{
    int sock, length, n;
    socklen_t fromlen;

    // A estrutura sockaddr_in contem um endereco de internet
    struct sockaddr_in server;
    struct sockaddr_in from;

    // Buffer para receber mensagens
    char buf[NBUFFERS*BUFFER_SIZE];

    //ary[i][j] is then rewritten as ary[i*sizeY+j]

    if (argc < 2)
    {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(0);
    }

    // Cria um socket do tipo datagrama e retorna um descritor
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0) error("Opening socket");

    // Zera a estrutura "server"
    bzero(&server, length);
    // Define a familia do endereco como do tipo Internet
    server.sin_family = AF_INET;
    // Seta o endereco para localhost (127.0.0.1)
    server.sin_addr.s_addr = INADDR_ANY;
    // A funcao htons() converte o numero da porta para o padrao Little Endian.
    server.sin_port = htons(atoi(argv[1]));

    length = sizeof(server);

    // Associa um socket a um endereco
    if (bind(sock, (struct sockaddr *) &server, length) < 0)
        error("binding");

    fromlen = sizeof(struct sockaddr_in);

    int front = 0, rear = 0;
    rear = BUFFER_SIZE;

    while (1)
    {
        // A funcao recvfrom() bloqueia o programa ate que um pacote seja lido.
        // Quando isso acontece, o pacote e' armazenado em um buffer passado por
        // parametro. Parametros: socket, buffer, tamanho do buffer, flags,
        // endereco da maquina que enviou o pacote, tamanho da estrutura do endereco.
        // Retorna o numero de bytes recebidos.
        n = recvfrom(sock, buf, BUFFER_SIZE, 0, (struct sockaddr *) &from, &fromlen);

        rear = ( rear + BUFFER_SIZE ) % ( NBUFFERS * BUFFER_SIZE );
        
        buf = *( buf + rear);

        if (n < 0) error("recvfrom");
    }
 }
