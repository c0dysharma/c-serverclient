#include <stdio.h>      //for prinf, fprintf, scanf and perror
#include <stdlib.h>     //for atoi() strings to int conversion
#include <string.h>     //for C style string operations
#include <sys/types.h>  //for defination of system Calls
#include <unistd.h>     //for read, write and close funciton on sockets
#include <sys/socket.h> //for creaing sockets and sockaddr structure
#include <netinet/in.h> //for internet domin address: struct sockaddr_in
#include <netdb.h>      //for ip lookup

#define bufferSize 255

//Just print out the descriptive error message(msg)
//into stderr and terminates the program
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]){
    if (argc < 3)
    {
        fprintf(stderr, "Not Enough option specified try again: %s serveraddress portnumber\n", argv[0]);
        fprintf(stderr, "Terminating.....\n");
        exit(1);
    }
    int sockfd;
    unsigned portNumber = atoi(argv[2]);    //atoi converts string into int
    char buffer[bufferSize];

    struct sockaddr_in serverAddress; //used to systemcall() for Network interface
    struct hostent * serverIP;        //for DNS query

    sockfd = socket(AF_INET, SOCK_STREAM, 0); //creation of ipv4, tcp steam and default tcp portocol socket
    if (sockfd < 0)
    {
        error("Error: Opening socket faild");
        exit(1);
    }
    printf("[0]Socket created Successfully....\n");

    //finding ip from hostname(if specified)
    serverIP = gethostbyname(argv[1]);
    printf(" Connection to %s ", (char *)serverIP->h_addr_list[0]);
     if (serverIP == NULL)
    {
        error("Error: IP lookup Faild.");
        exit(1);
    }
    bzero((char *)&serverAddress, sizeof(serverAddress)); //zero out the serverAddress of sizeof(serverAddress)
    
    //Setting Connection Properties
    serverAddress.sin_family = AF_INET; //telling to use IPv4

    // inet_aton("127.0.0.1", &serverAddress.sin_addr);
    bcopy((char * )serverIP->h_addr_list[0], (char *) &serverAddress.sin_addr.s_addr, serverIP->h_length); //copies previously received IP address into Server Propeties List
    serverAddress.sin_port = htons(portNumber); //to convert system eindnness to sutiable eindanness

    //connectiion to the server
    int connection = connect(sockfd, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if(connection<0){
        printf("Error: Connection to %s faild",(char *)serverIP->h_addr_list);
        exit(1);
    }
    printf("[1]Connection Established Successfully....\n");
    printf("Type exit() to disconnect\n");

    while (1)
    {
        bzero(buffer, bufferSize);
        printf("You: ");
        fgets(buffer, bufferSize, stdin);
        
        int n = write(sockfd, buffer, strlen(buffer));
        if (n < 0)
        {
            error("Error: Writing Buffer Faild.");
        }
        else if (!(strcmp("exit()\n", buffer)))
        {
            break;
        }

        bzero(buffer, bufferSize);
        n = read(sockfd, buffer, bufferSize);
        if (n < 0)
        {
            printf("Error: Buffer Reading Faild.");
        }
        printf("Server: %s\n", buffer);

        if (!(strcmp("exit()\n", buffer)))
        {
            break;
        }
    }
    close(sockfd);
    return 0;
}