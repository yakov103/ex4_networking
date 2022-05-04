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

#define SERVER_PORT 5050 //The port that the server listens



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
    struct sockaddr_in client_addr;
    socklen_t clientAdressLen = sizeof(client_addr);

    for (int i = 0 ; i < 2 ; i++){ 
        if (sock == -1 ){
            printf("Error accepting connection\n");
            exit(1);
        }
        socklen_t len = sizeof(buffer);
        if (getsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, buffer, &len) < 0){
            printf("Error getting socket option\n");
            exit(1);
        }
        strcpy(buffer, i > 0 ? "reno" : "cubic");
        len = sizeof(buffer);
        if (getsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, buffer, &len) < 0){
            printf("Error setting socket option\n");
            exit(1);
        }
        len = sizeof(buffer);
        if (getsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, buffer, &len) < 0){
            printf("Error getting socket option\n");
            exit(1);
        }
        double avg = 0 ; 
        for (int j = 0 ; j < 5 ; j++ ){
            clientAdressLen  = sizeof(client_addr);
            int client_sock = accept(sock, (struct sockaddr *) &client_addr, &clientAdressLen);
            if (client_sock < 0){
                printf("Error accepting connection\n");
                exit(1);
            }
            //Receive data from the client
            printf("Start timer \n");
            struct timespec start, end;
            clock_gettime(CLOCK_REALTIME, &start); // need to check that this works

            int bytes_read = -1; 
            while (bytes_read != 0){
                memset(buffer, 0, sizeof(buffer));
                bytes_read = recv(client_sock, buffer, 1024, 0);
            }
            clock_gettime(CLOCK_REALTIME, &end);
            double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
            printf("Time for package %d is %f \n", i+j, time_taken);

            avg += time_taken;

            sleep(1);
        


        }

    printf("\n Avg time for %s is %f \n", buffer, avg/5);
    }


    close(sock);
    return 0 ; 
}