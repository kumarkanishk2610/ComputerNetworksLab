#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[1024];
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &(serverAddr.sin_addr));
    connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    recv(clientSocket, buffer, sizeof(buffer), 0);
    printf("Received date and time from server: %s\n", buffer);
    close(clientSocket);
    return 0;
}
