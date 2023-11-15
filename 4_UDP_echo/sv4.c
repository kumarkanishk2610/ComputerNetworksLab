#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(struct sockaddr_in);
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 5);
    printf("Server listening on port %d...\n", 12345);
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
    printf("Connection accepted from %s:%d\n",
           inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
    char buffer[1024];
    recv(clientSocket, buffer, sizeof(buffer), 0);
    printf("Received from client: %s\n", buffer);
    send(clientSocket, buffer, strlen(buffer), 0);
    close(clientSocket);
    close(serverSocket);
    return 0;
}
