#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
int main() {
    int client_socket;
    struct sockaddr_in server_addr = {AF_INET, htons(8080), inet_addr("127.0.0.1")};
    char message[1024];

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    printf("Enter a message: ");
    fgets(message, sizeof(message), stdin);
    send(client_socket, message, strlen(message), 0);

    close(client_socket);
    return 0;
}
