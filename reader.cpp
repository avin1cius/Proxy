#include "reader.h"

Reader::Reader( char &b) : buf(b)
{
    priority = false;
    executionTime = 0;
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void Reader::run(int argc, char *argv[])
{
    int sock, n;
    unsigned int length;

    // A estrutura sockaddr_in contem um endereco de internet
    struct sockaddr_in server, from;

    // A estrutura hostent representa um host na Internet
    struct hostent *hp;

    if (argc != 3)
    {
        printf("Usage: server port\n");
        exit(1);
    }

    // Cria um socket do tipo datagrama e retorna um descritor
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0) error("socket");

    // Define a familia do endereco como do tipo Internet
    server.sin_family = AF_INET;

    // Preenche a estrutura "hp" a partir do nome da maquina ou de seu IP
    hp = gethostbyname(argv[1]);

    if (hp == 0) error("Unknown host");

    // Copia o IP da estrutura "hp" para a estrutura "server"
    bcopy( (char *) hp->h_addr, (char *) &server.sin_addr, hp->h_length);
    // A funcao htons() converte o numero da porta para o padrao Little Endian.
    server.sin_port = htons(atoi(argv[2]));

    length = sizeof(struct sockaddr_in);

    printf("Please enter the message: ");

    // Zera o buffer
    bzero(buffer, BUFFER_SIZE);

    fgets(buffer, 255, stdin);

    // Envia dados pela rede. Parametros: socket, buffer que contem os dados,
    // tamanho do buffer, flags, endereco da maquina destino, tamanho da estrutura do endereco.
    // Retorna o numero de bytes enviados.
    n = sendto(sock, buffer, strlen(buffer), 0, (const struct sockaddr *) &server, length);

    if (n < 0) error("Sendto");

    // A funcao recvfrom() bloqueia o programa ate que um pacote seja lido.
    // Quando isso acontece, o pacote e' armazenado em um buffer passado por
    // parametro. Parametros: socket, buffer, tamanho do buffer, flags,
    // endereco da maquina que enviou o pacote, tamanho da estrutura do endereco.
    // Retorna o numero de bytes recebidos.
    n = recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &from, &length);

    if (n < 0) error("recvfrom");

    write(1, "Got an ack: ", 12);
    write(1, buffer, n);

    close(sock);
}
