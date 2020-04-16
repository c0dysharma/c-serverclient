#include <stdio.h>      //for prinf, fprintf, scanf and perror
#include <stdlib.h>     //for atoi() strings to int conversion
#include <string.h>     //for C style string operations
#include <sys/types.h>  //for defination of system Calls
#include <unistd.h>     //for read, write and close funciton on sockets
#include <sys/socket.h> //for creaing sockets and sockaddr structure
#include <netinet/in.h> //for internet domin address: struct sockaddr_in
#define bufferSize 255

//Just print out the descriptive error message(msg)
//into stderr and terminates the program
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "No port number specified try again: %s portnumber\n", argv[0]);
        fprintf(stderr, "Terminating.....\n");
        exit(1);
    }

    //Just required variables
    int sockfd;
    unsigned portNumber;
    char buffer[bufferSize];

    struct sockaddr_in serverAddress, clientAddress; //used to systemcall() for Network interface
    socklen_t clientLen;                             //a type needed by bind() to get the size

    sockfd = socket(AF_INET, SOCK_STREAM, 0); //creation of ipv4, tcp steam and default tcp portocol socket
    if (sockfd < 0)
    {
        error("Error: Opening socket faild");
        exit(1);
    }
    printf("[0]Socket created Successfully....\n");
    bzero((char *)&serverAddress, sizeof(serverAddress));
    portNumber = atoi(argv[1]);

    //serverAddress properties for future binding
    serverAddress.sin_family = AF_INET;         //IPv4
    serverAddress.sin_port = htons(portNumber); //to convert system eindnness to sutiable eindanness
    serverAddress.sin_addr.s_addr = INADDR_ANY; //use any local IP or the server

    //Binding the socket framework with the ip address:portNumber
    int binding = bind(sockfd, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if (binding < 0)
    {
        error("Error: Binding Faild.");
        exit(1);
    }
    printf("[1]IP Bounded Successfully....\n");
    //setting up a listener on server with backlog of 5
    listen(sockfd, 5);
    printf("[2]Listening on port %u Successfully....\n", portNumber);

    //determing the size of client connectio for accepting the connection
    clientLen = sizeof(clientAddress);
    sockfd = accept(sockfd, (struct sockaddr *)&clientAddress, &clientLen);

    if (sockfd < 0)
    {
        error("Error: Aceepting Connection Faild.\n");
    }
    printf("[3]Conection Established Successfully....\n");

    //Read and Write functions in an endless loop.
    while (1)
    {
        bzero(buffer, bufferSize);

        int n = read(sockfd, buffer, bufferSize);
        if (n < 0)
        {
            error("Error: Reading Buffer Faild.");
        }
        printf("Client: %s\n", buffer);

        bzero(buffer, bufferSize);
        printf("You: ");
        fgets(buffer, bufferSize, stdin);
        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0)
        {
            printf("Error: Buffer Writting Faild.");
        }

        if (!(strcmp("exit()", buffer)))
        {
            break;
        }
    }
    close(sockfd);
    return 0;
}
