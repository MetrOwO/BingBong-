#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>
/*
Comment and documentations? Fuck Off... I don't even know how this works and if it will works in the future 
*/

#define PORT 8080


//I Feel Like Jesus
int initialiseSocketClient(char* ip_address) {
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;

    if (WSAStartup(MAKEWORD(2, 2), &wsa)) {
        printf("WSAStartup failed: %d\n", WSAGetLastError());
        return -1;
    }

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Socket creation failed: %d\n", WSAGetLastError());
        WSACleanup();
        return -2;
    }

    server.sin_addr.s_addr = inet_addr(ip_address);
    if (server.sin_addr.s_addr == INADDR_NONE) {
        printf("Invalid IP address: %s\n", ip_address);
        closesocket(s);
        WSACleanup();
        return -1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    if (connect(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Connect error: %d\n", WSAGetLastError());
        closesocket(s);
        WSACleanup();
        return -1;
    }

    puts("Connected");
    closesocket(s);
    WSACleanup();
    return 0;
}

//Bing Bong can talk with his friends... How Beautiful is life
int createServer() {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server, client;
    int c;

    printf("Inizializzazione di Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Errore Winsock: %d\n", WSAGetLastError());
        return -1;
    }
    printf("Winsock inizializzato.\n");

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Errore creazione socket: %d\n", WSAGetLastError());
        WSACleanup();
        return -2;
    }
    printf("Socket creato.\n");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Errore bind: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return -3;
    }
    printf("Socket associato alla porta %d.\n", PORT);

    if (listen(server_socket, 3) == SOCKET_ERROR) {
        printf("Errore listen: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return -4;
    }
    printf("In ascolto sulla porta %d...\n", PORT);

    c = sizeof(struct sockaddr_in);
    client_socket = accept(server_socket, (struct sockaddr *)&client, &c);
    if (client_socket == INVALID_SOCKET) {
        printf("Errore accettazione connessione: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return -5;
    }

    printf("Connessione accettata.\n");
    printf("Client connesso: %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();
    return 0;
}
