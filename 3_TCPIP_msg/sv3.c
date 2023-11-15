#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr = {AF_INET, htons(8080), INADDR_ANY};
    socklen_t addr_size = sizeof(struct sockaddr_in);
    char buffer[1024];
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_socket, 5);
    client_socket = accept(server_socket, (struct sockaddr*)&server_addr, &addr_size);
    recv(client_socket, buffer, sizeof(buffer), 0);
    buffer[strcspn(buffer, "\n")] = '\0';
    printf("Received message from client: %s\n", buffer);
    close(client_socket);
    close(server_socket);
    return 0;
}
