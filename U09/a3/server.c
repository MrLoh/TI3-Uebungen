// this is based on code from http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define BACKLOG 1     // number of pending connections queue will hold
#define BUFFSIZE 100  // size of receive and send buffer
char buff[BUFFSIZE+1];

int send_file(int sockfd, char* filename, int flags)
{
    FILE *file;
    int bytes = 0;
    if((file = fopen(filename, "r")) == NULL)
        return -1;
    while (!feof(file))
    {
        memset(buff, 0, BUFFSIZE);
        fgets(buff, BUFFSIZE, file);
        bytes += send(sockfd, buff, BUFFSIZE, flags);
    }
    return bytes;
}

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
    int sock;
    if((sock = socket(addrinfo->ai_family, addrinfo->ai_socktype, addrinfo->ai_protocol)) == -1)
    {
        perror("server: socket");
        return 3;
    }

    // allow reuse of address
    int yes = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
        perror("server: setsockopt");
        return 4;
    }

    // bind socket
    if(bind(sock, addrinfo->ai_addr, addrinfo->ai_addrlen) == -1)
    {
        perror("server: bind");
        return 4;
    }
    freeaddrinfo(addrinfo);
    printf("Server started at localhost:%s.\n", argv[1]);

    // make server listening
    if (listen(sock, BACKLOG) == -1)
    {
        perror("listen");
        return 5;
    }


    int connection;
    while(1) // accept loop
    {
        struct sockaddr *client_addr;
        socklen_t client_addr_size = sizeof client_addr;
        if((connection = accept(sock, (struct sockaddr *)&client_addr, &client_addr_size)) == -1)
        {
            perror("server: accept");
            continue;
        }
        printf("Server connected to client.\n");
        while(1) // recv-send loop
        {
            memset(buff, 0, BUFFSIZE);
            if(recv(connection, buff, BUFFSIZE, 0) != 0)
            {
                printf("Message received: '%s'\n", buff);
            }

            // Terminate Connection
            if(!strncmp(buff,"Stopp",BUFFSIZE))
            {
                shutdown(connection, 2);
                printf("Connection to client closed.\n");
                break;
            }
            // Greeting
            else if(!strncmp(buff,"Hi",BUFFSIZE))
            {
                send(connection, "Hi there, good to meet you. How can I help you?", BUFFSIZE, 0);
                send(connection, "</end>", BUFFSIZE, 0);
                printf("Response 1 send.\n");
            }
            // Jingle Bells Lyrics
            else if(!strncmp(buff,"Dashing through the snow",BUFFSIZE))
            {
                send(connection, "In a one-horse open sleigh", BUFFSIZE, 0);
                send(connection, "O'er the fields we go", BUFFSIZE, 0);
                send(connection, "Laughing all the way", BUFFSIZE, 0);
                send(connection, "</end>", BUFFSIZE, 0);
                printf("Response 2 send.\n");
            }
            else if(!strncmp(buff,"Bells on bobtail ring",BUFFSIZE))
            {
                send(connection, "Making spirits bright", BUFFSIZE, 0);
                send(connection, "What fun it is to ride and sing", BUFFSIZE, 0);
                send(connection, "A sleighing song tonight!", BUFFSIZE, 0);
                send(connection, "</end>", BUFFSIZE, 0);
                printf("Response 3 send.\n");
            }
            else if(!strncmp(buff,"Jingle bells, jingle bells,",BUFFSIZE))
            {
                send(connection, "Jingle all the way!", BUFFSIZE, 0);
                send(connection, "</end>", BUFFSIZE, 0);
                printf("Response 4 send.\n");
            }
            else if(!strncmp(buff,"Oh, what fun it is to ride",BUFFSIZE))
            {
                send(connection, "In a one-horse open sleigh.", BUFFSIZE, 0);
                send(connection, "</end>", BUFFSIZE, 0);
                printf("Response 5 send.\n");
            }
            else if(!strncmp(buff,"Hey",BUFFSIZE))
            {
                send(connection, "Jingle bells, jingle bells,", BUFFSIZE, 0);
                send(connection, "Jingle all the way!", BUFFSIZE, 0);
                send(connection, "Oh! what fun it is to ride", BUFFSIZE, 0);
                send(connection, "In a one-horse open sleigh.", BUFFSIZE, 0);
                send(connection, "</end>", BUFFSIZE, 0);
                printf("Response 6 send.\n");
            }
            // Christmassy ASCII Pictures
            else if(!strncmp(buff,"santa?",BUFFSIZE) ||
                    !strncmp(buff,"santa clause?",BUFFSIZE))
            {
                send_file(connection, "_santa.txt", 0);
                send(connection, "</end>", BUFFSIZE, 0);
                printf("Picture 1 send.\n");
            }
            else if(!strncmp(buff,"jesus?",BUFFSIZE) ||
                    !strncmp(buff,"manger?",BUFFSIZE) ||
                    !strncmp(buff,"nativity scene?",BUFFSIZE))
            {
                send_file(connection, "_manger.txt", 0);
                send(connection, "</end>", BUFFSIZE, 0);
                printf("Picture 2 send.\n");
            }
            else if(!strncmp(buff,"tree?",BUFFSIZE) ||
                    !strncmp(buff,"christmas tree?",BUFFSIZE) ||
                    !strncmp(buff,"tannenbaum?",BUFFSIZE))
            {
                send_file(connection, "_tree.txt", 0);
                send(connection, "</end>", BUFFSIZE, 0);
                printf("Picture 3 send.\n");
            }
            else if(!strncmp(buff,"angel?",BUFFSIZE))
            {
                send_file(connection, "_angel.txt", 0);
                send(connection, "</end>", BUFFSIZE, 0);
                printf("Picture 4 send.\n");
            }
            else if(!strncmp(buff,"snowman?",BUFFSIZE) ||
                    !strncmp(buff,"frosty?",BUFFSIZE))
            {
                send_file(connection, "_snowman.txt", 0);
                send(connection, "</end>", BUFFSIZE, 0);
                printf("Picture 5 send.\n");
            }
            else if(!strncmp(buff,"nutcracker?",BUFFSIZE))
            {
                send_file(connection, "_nutcracker.txt", 0);
                send(connection, "</end>", BUFFSIZE, 0);
                printf("Picture 6 send.\n");
            }
            else if(!strncmp(buff,"candle?",BUFFSIZE))
            {
                send_file(connection, "_candle.txt", 0);
                send(connection, "</end>", BUFFSIZE, 0);
                printf("Picture 7 send.\n");
            }
            // Retry Message
            else if(strncmp(buff,"",BUFFSIZE))
            {
                send(connection, "</end>", BUFFSIZE, 0);
                printf("Retry message send.\n");
            }
        }

    }
    return 0;
}