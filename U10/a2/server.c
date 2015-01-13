
/*
this is inspired by:
http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html
http://blog.abhijeetr.com/2010/04/very-simple-http-server-writen-in-c.html

1. Oeffnen Sie einen Server-Socket.
2. Versetzen Sie den Socket in den Listen-Status und merken Sie sich die Rueckgabe (den Deskriptor).
3. Machen Sie sich mit dem Konzept von select() vertraut.
4. Nutzen Sie FD_ZERO um eine leere Socket-Struktur zu erstellen.
5. Nutzen Sie FD_SET um ihren Server-Socket an die erste Stelle der Struktur zu schreiben.
6. Programmieren Sie eine Endlosschleife mit folgendem Inhalt:
    (a) Kopieren Sie ihre Socket-Struktur.
    (b) Rufen Sie auf der Kopie select() auf.
    (c) Pruefen Sie ob auf dem Server-Socket ein Verbindungswunsch aufgelaufen ist (FD ISSET), wenn ja nehmen Sie Ihn mittels accept() an und merken Sie sich den Filedescriptor!
    (d) Pruefen Sie alle Ihre Filedescriptoren (von den Clients) in einer Schleife darauf, ob sie in Ihrer Kopie der Socket-Struktur enthalten sind (FD ISSET). Wenn ja lesen Sie mittels read() von diesem Descriptor die Anfrage ein.
    (e) Parsen Sie den Dateinamen aus der Anfrage und geben Sie eine entsprechende Ant- wort (Grafik, Page oder Fehler), inkl. Header zurueck.
    (f) Schliessen Sie die Verbindung!
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
    ROOT = getenv("PWD");

    char port[5];
    if( argc < 2 )
        strcpy(port, "8080");
    else
        strcpy(port, argv[2]);

    // construct address info
    int status;
    struct addrinfo hints, *addrinfo;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;     // accept IPv4 and IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE;     // fill in local IP
    if( (status = getaddrinfo(NULL, port, &hints, &addrinfo)) != 0 )
    {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        return 2;
    }

    // create socket
    int listener;
    if( (listener = socket(addrinfo->ai_family, addrinfo->ai_socktype, addrinfo->ai_protocol)) == -1 )
    {
        perror("server: socket");
        return 3;
    }

    // allow reuse of address
    int yes = 1;
    if( setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1 )
    {
        perror("server: setsockopt");
        return 4;
    }

    // bind socket
    if( bind(listener, addrinfo->ai_addr, addrinfo->ai_addrlen) == -1 )
    {
        perror("server: bind");
        return 4;
    }
    freeaddrinfo(addrinfo);
    printf("\nServer started at localhost:%s.\n", port);

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
