/*   TCP/IP-server*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <time.h>

#define SERVER_PORT 8080 //The port that the server listens

int main (){
    char buffer[1024];
    //Create a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0){
        printf("Error creating socket\n");
        exit(1);
    }
    //Reuse the socket if the server was closed 
    // and remains for 45 seconds in the TIME_WAIT state

    int reuse = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0){
        printf("Error setting socket option\n");
        exit(1);
    }
    //"sockaddr_in" is a the delivered structure for the socket address
    // used for IPv4 communication . for IPv6, use "sockaddr_in6"

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; 
    server_addr.sin_port = htons(SERVER_PORT);

    //Bind the socket to the port
    if (bind(sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0){
        printf("Error binding socket\n");
        exit(1);
    }
    //Listen for connections 
    // Make the socket a listening socket
    if (listen(sock, 500) < 0){ // 500 is the maximum number of queued connections
        printf("Error listening\n");
        exit(1);
    }
    //Accept a connection
    printf("Waiting for connections\n");
    socklen_t client_addr_size = sizeof(server_addr);

    


    return 0 ; 
}