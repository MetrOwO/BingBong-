#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#define PORT 8080

int initialiseSocket(char* ip_address){
    /*
    0 --> ok
    -1 --> Generic error
    -2 --> Could not create socket
    */
    
    WSADATA wsa;
    SOCKET s;
    struct socketaddr_in server;


    if(WSAStartup(MAKEWORD(2,2),&wsa)){
        return -1;
    }

    if((s = socket(AF_INET,SOCK_STREAM,0)) == INVALID_SOCKET) return -2


    server.sin_addr.s_addr = inet_addr(ip_address)
    server.sin_family = AF_INET;
    server.sin_port = htons(80);

    if(connect(s,(struct sockaddr *)&server,sizeof(server)) < 0){
        puts("Connect error");
        return -1;
    }
    puts("Connected");
    return 0

}