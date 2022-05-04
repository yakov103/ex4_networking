/* TCP/IP client */ 


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

#define SERVER_Port 5050
#define SERVER_IP "127.0.0.1"

#define MBtoByte 1048576 // 1048576 -> 1MB TO 1048576 Bytes

int main(){ 
    char buffer[1024];
    for(int i = 0 ; i < 2 ; i++ ){ 
        for(int j = 0 ; j < 5 ; j++ ){
            int sock = socket(AF_INET, SOCK_STREAM, 0);
            if(sock < 0){
                printf("Error creating socket\n");
                exit(1);
            }
            socklen_t len = sizeof(buffer);
            if (getsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, buffer, &len) < 0) {
                printf("Error getting socket option\n");
                exit(1);
            }
            strcpy(buffer, i > 0 ? "reno" : "cubic");
            len = sizeof(buffer);
            if (getsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, buffer, &len ) < 0) {
                printf("Error setting socket option\n");
                exit(1);
            }
            len = sizeof(buffer);
            if (getsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, buffer, &len) < 0) {
                printf("Error getting socket option\n");
                exit(1);
            }
            // "sockaddr_in" is a the delivered structure for the socket address
            // used for IPv4 communication . for IPv6, use "sockaddr_in6"

            struct sockaddr_in server_addr;
            memset(&server_addr, 0, sizeof(server_addr));
            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(SERVER_Port);
            int rval = inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);
            if (rval == 0) {
                printf("inet_pton(): Invalid address\n");
                exit(1);
            } else if (rval < 0) {
                printf("inet_pton(): Error\n");
                exit(1);
            }
            // Make a connection to the server using the socket and the address
            if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
                printf("Error connecting\n");
                continue;
            }

            printf("Connected to server\n");
            // Send the message to the server
            FILE *fp = fopen("./1mb.txt", "r");

            if(fp == NULL){
                printf("Error opening file\n");
                exit(1);
            }
            int data_strm; 
            int total_bytes = 0;
            while( 0 < (data_strm = fread(buffer, 1, sizeof(buffer), fp))){
               int bytes_sent = send(sock, buffer, data_strm, 0);
                if(bytes_sent < 0){
                     printf("Error sending data\n");
                     exit(1);
                }
                total_bytes += bytes_sent;
            }
            if (total_bytes != MBtoByte) {
                printf("Error sending data\n");
                continue;
            }
            printf("Sent %d bytes\n", total_bytes);
            sleep(1); 
            close(sock);

        }
        
    }


return 0 ; 

}