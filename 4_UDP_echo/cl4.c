#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    printf("Connected to server\n");
    char message[1024];
    printf("Enter message to send to server: ");
    fgets(message, sizeof(message), stdin);
    send(clientSocket, message, strlen(message), 0);
    recv(clientSocket, message, sizeof(message), 0);
    printf("Received from server: %s\n", message);
    close(clientSocket);
    return 0;
}
