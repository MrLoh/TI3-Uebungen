/*
Da c99 aus unerfindlichen Gründen getaddrinfo nicht unterstützt, ist diese Variante unter c99 zu verwenden. Generell ist die Nutzung von getaddrinfo aber empfehlenswert.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netdb.h>
#include <fcntl.h>
#include <arpa/inet.h>



#define BACKLOG 32     // number of pending connections queue will hold
#define BUFFSIZE 1024  // size of receive and send buffer
char *ROOT;
char buff[BUFFSIZE+1];



long fsize(FILE *file)
{
    fseek(file, 0, SEEK_END);
    long len = ftell(file);
    fseek(file, 0, SEEK_SET);
    return len;
}



char *fmime(char *path)
{
    static char mime[20];
    char extension[10];
    strcpy(extension, strrchr(path, '.')+1);
    if( strncmp(extension, "jpg", 4) == 0 )
        strcpy(extension, "jpeg");
    if( strncmp(extension, "svg", 4) == 0 )
        strcpy(extension, "svg+xml");
    if( strncmp(extension, "htm", 4) == 0 )
        strcpy(extension, "html");
    if( strncmp(extension, "txt", 4) == 0 )
        strcpy(extension, "plain");
    if( strncmp(extension, "jpeg", 5) == 0 ||
        strncmp(extension, "gif", 4) == 0 ||
        strncmp(extension, "png", 4) == 0 ||
        strncmp(extension, "svg+xml", 8) == 0 )
    {
        strcpy(mime, "image/");
        strcat(mime, extension);
    }
    else if( strncmp(extension, "html", 5) == 0 ||
             strncmp(extension, "css", 4) == 0 ||
             strncmp(extension, "plain", 6) == 0 )
    {
        strcpy(mime, "text/");
        strcat(mime, extension);
    }
    else
        return NULL;
    return mime;
}



void response_handler(int connection, fd_set *master)
{
    memset(buff, 0, BUFFSIZE);
    if( recv(connection, buff, BUFFSIZE, 0) != 0 )
    {
        char request[BUFFSIZE];
        strcpy(request, strtok(buff, "\n"));
        printf("\nMessage received: \n\t%s\n\t%s\n", request, strtok(NULL, "\n"));
        strtok(request, " \n");
        if( strncmp(request, "GET\0", 4) != 0 )
        {
            send(connection, "HTTP/1.0 405 Method Not Allowed\nAllow: GET\n\n", 44, 0);
            printf("\nMessage send: \n\tHTTP/1.0 405 Method Not Allowed\n");
        }
        else
        {
            char path[BUFFSIZE];
            char protocol[BUFFSIZE];
            FILE *file;
            strcpy(path, strtok(NULL, " \t"));
            ;
            if( strncmp(strrchr(path, '/'), "/\0", 2) == 0 )
                strcat(path, "index.html");
            else if( strrchr(path, '.') == NULL )
                strcat(path, "/index.html");
            strcpy(protocol, strtok(NULL, " \t\n"));
            char *mime = fmime(path);
            printf("%s\n", path);
            if ( strncmp(protocol, "HTTP/1.0", 8) != 0 && strncmp(protocol, "HTTP/1.1", 8) != 0 )
            {
                send(connection, "HTTP/1.0 400 Bad Request\n", 25, 0);
                printf("\nMessage send: \n\tHTTP/1.0 400 Bad Request\n");
            }
            else if( mime == NULL )
            {
                send(connection, "HTTP/1.0 415 Unsupported Media Type\n\n", 37, 0);
                printf("\nMessage send: \n\tHTTP/1.0 415 Unsupported Media Type\n");
            }
            else if( (file = fopen(path+1, "r")) == NULL )
            {
                send(connection, "HTTP/1.0 404 Not Found\n\n", 24, 0);
                printf("\nMessage send: \n\tHTTP/1.0 404 Not Found\n");
            }
            else
            {
                long len = fsize(file);
                send(connection, "HTTP/1.0 200 OK\n", 16, 0);
                printf("\nMessage send: \n\tHTTP/1.0 200 OK\n");
                memset(buff, 0, BUFFSIZE);
                snprintf(buff, BUFFSIZE, "Content-Type: %s\nContent-Length: %ld\n", mime, len);
                send(connection, buff, strlen(buff), 0);
                printf("\t%s\n\t%s\n", strtok(buff, "\n"), strtok(NULL, "\n"));
                send(connection, "Connection: close\n\n", 19, 0);
                printf("\tConnection: close\n\n");
                int bytes_read, bytes_send;
                memset(buff, 0, BUFFSIZE);
                while( (bytes_read = fread(buff, 1, BUFFSIZE, file)) > 0 )
                {
                    bytes_send = 0;
                    while( (bytes_send += send(connection, buff+bytes_send, bytes_read, 0)) < bytes_read );
                    memset(buff, 0, BUFFSIZE);
                }

                printf("\tcontent of %s\n", path+1);
                fclose(file);
            }
        }
    }
    shutdown(connection, 2);
    FD_CLR(connection, master);
    printf("\nConnection closed.\n");
}



void connection_handler(int listener, fd_set *master, int *fdmax)
{
    int connection;
    struct sockaddr *client_addr;
    socklen_t client_addr_size = sizeof client_addr;
    if((connection = accept(listener, (struct sockaddr *)&client_addr, &client_addr_size)) == -1)
    {
        perror("server: accept");
        return;
    }
    else
    {
        FD_SET(connection, master);
        if( connection > *fdmax )
            *fdmax = connection;
        printf("\nNew Connection to Server.\n");
    }
}



int main(int argc, char const *argv[])
{

    int listener;
    struct sockaddr_in my_addr;

    listener = socket(PF_INET, SOCK_STREAM, 0);

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(8080);     // short, network byte order
    my_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);

    if( bind(listener, (struct sockaddr *)&my_addr, sizeof my_addr) == -1 )
    {
        perror("server: bind");
        return 4;
    }
    printf("\nServer started at localhost: 8080.\n");

    // make server listening
    if( listen(listener, BACKLOG) == -1 )
    {
        perror("listen");
        return 5;
    }

    fd_set master;    // master file descriptor list
    fd_set read_fds;  // master file descriptor list
    int fdmax;        // maximum file descriptor number
    FD_ZERO(&master);
    FD_ZERO(&read_fds);

    FD_SET(listener, &master);
    fdmax = listener;

    while( 1 )
    {
        fd_set read_fds = master; // copy of master
        if( select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1 )
        {
            perror("select");
            return 10;
        }

        for( int connection = 0; connection <= fdmax; connection++ )
        {
            if( FD_ISSET(connection, &read_fds) )
            {
                if( connection == listener )
                    connection_handler(listener, &master, &fdmax);
                else
                    response_handler(connection, &master);
            }
        }
    }
}
