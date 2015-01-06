// this is based on code from http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define BACKLOG 1     // number of pending connections queue will hold
#define BUFFSIZE 100 // size of receive and send buffer

int main(int argc, char const *argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "wrong number of arguments.\n");
        return 1;
    }

    // construct address info
    int status;
    struct addrinfo hints, *addrinfo;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;     // accept IPv4 and IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE;     // fill in local IP
    if((status = getaddrinfo(NULL, argv[1], &hints, &addrinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        return 2;
    }

    // create socket
    int connection;
    if((connection = socket(addrinfo->ai_family, addrinfo->ai_socktype, addrinfo->ai_protocol)) == -1)
    {
        perror("client: socket");
        return 3;
    }

    if(connect(connection, addrinfo->ai_addr, addrinfo->ai_addrlen) == -1)
    {
        shutdown(connection, 2);
        perror("client: connect");
        return 6;
    }
    freeaddrinfo(addrinfo);
    printf("Client connected to localhost:%s.\n", argv[1]);


    char buff[BUFFSIZE+1];
    while(1) // send-recv loop
    {
        printf("\n");
        memset(buff, 0, BUFFSIZE);
        printf("\tsend:\t");
        fgets(buff, BUFFSIZE, stdin);
        sscanf(buff, "%[^\n]", buff);
        send(connection, buff, BUFFSIZE, 0);
        if(!strncmp(buff,"Stopp",BUFFSIZE))
        {
            shutdown(connection, 2);
            printf("\nClosing connection to server.\n");
            return 0;
        }
        printf("\trecv:\t");
        recv(connection, buff, BUFFSIZE, 0);
        while(strncmp(buff, "</end>", BUFFSIZE))
        {
            printf("%s\n", buff);
            recv(connection, buff, BUFFSIZE, 0);
            printf("\t\t");
        }
    }
}

