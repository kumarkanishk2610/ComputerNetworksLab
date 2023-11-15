#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
void handle_client(int client_socket) {
    char buffer[1024];
    ssize_t bytes_received;
    const char* welcome = "Welcome to the FTP server.\n";
    send(client_socket, welcome, strlen(welcome), 0);
    while (1) {
        bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) break;
        buffer[bytes_received] = '\0';
        if (strcmp(buffer, "UPLOAD") == 0) {
            FILE* file = fopen("uploaded_file.txt", "wb");
            while ((bytes_received = recv(client_socket, buffer, sizeof(buffer), 0)) > 0) {
                fwrite(buffer, 1, bytes_received, file);
            }
            fclose(file);
            printf("File uploaded successfully.\n");
        } else {
            printf("Received: %s", buffer);
            send(client_socket, buffer, bytes_received, 0);
        }
    }
    close(client_socket);
}
int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_len = sizeof(client_address);
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(12345);
    bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    listen(server_socket, 5);
    printf("FTP Server listening on port %d...\n", 12345s);
    while (1) {
        client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_len);
        handle_client(client_socket);
    }
    close(server_socket);
    return 0;
}
