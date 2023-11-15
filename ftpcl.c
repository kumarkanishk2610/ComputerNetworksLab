#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
void upload_file(int socket, const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    const char* upload_command = "UPLOAD";
    send(socket, upload_command, strlen(upload_command), 0);
    char buffer[1024];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        send(socket, buffer, bytes_read, 0);
    }
    fclose(file);
    printf("File uploaded successfully.\n");
}
int main() {
    int client_socket;
    struct sockaddr_in server_address;
    char buffer[1024];
    ssize_t bytes_received;
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(12345);
    connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    buffer[bytes_received] = '\0';
    printf("Server: %s", buffer);
    const char* command = "Hello, server!\n";
    send(client_socket, command, strlen(command), 0);
    bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    buffer[bytes_received] = '\0';
    printf("Server Echo: %s", buffer);
    const char* filename = "example_file.txt";
    upload_file(client_socket, filename);
    close(client_socket);
    return 0;
}
